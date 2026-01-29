#include "alcove_util.hpp"
#include <fmt/format.h>
#include <sstream>

using namespace alcove;

// Core

result alcove::add_record(const std::string& ip, const std::string& domain, int* out_id) {
    int id;
    if (auto result = util::find_next_id(id); result != result::SUCCESS) {
        return result;
    }

    auto hosts = util::open_hosts_for_writing();
    if (!hosts.is_open()) {
        return result::HOSTS_WRITE_FAILED;
    }

    hosts << fmt::format("{} {} {}\n", ip, domain, util::format_tag(id));

    fmt::println("Added record {} | {} -> {}", id, ip, domain);

    if (out_id != nullptr) {
        *out_id = id;
    }

    return result::SUCCESS;
}

result alcove::delete_record(int id) {
    auto read_hosts = util::open_hosts_for_reading();
    if (!read_hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    const auto tag = util::format_tag(id);

    std::vector<std::string> lines = util::read_lines(read_hosts);
    read_hosts.close();

    auto write_hosts = util::open_hosts_for_writing(std::ios::trunc);
    if (!write_hosts.is_open()) {
        return result::HOSTS_WRITE_FAILED;
    }

    bool found = false;
    for (const auto& line : lines) {
        if (auto tag_pos = line.find(tag); tag_pos != std::string::npos) {
            if (auto id = util::find_id(line); id < 0) {
                // this should never happen
                fmt::println("Delected record {} but failed to find ID", line);
            }
            else {
                fmt::println("Deleted record {}", id);
            }

            found = true;
            continue;
        }

        write_hosts << line << '\n';
    }

    if (!found) {
        return result::RECORD_NOT_FOUND;
    }

    return result::SUCCESS;
}

result alcove::clear_records() {
    auto read_hosts = util::open_hosts_for_reading();
    if (!read_hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    const auto tag = util::get_tag_prefix();
    
    std::vector<std::string> lines = util::read_lines(read_hosts);
    read_hosts.close();

    auto write_hosts = util::open_hosts_for_writing(std::ios::trunc);
    if (!write_hosts.is_open()) {
        return result::HOSTS_WRITE_FAILED;
    }

    for (const auto& line : lines) {
        if (line.find(tag) != std::string::npos) {
            if (auto id = util::find_id(line); id < 0) {
                // this should never happen
                fmt::println("Delected record {} but failed to find ID", line);
            }
            else {
                fmt::println("Deleted record {}", id);
            }
            
            continue;
        }

        write_hosts << line << '\n';
    }

    return result::SUCCESS;
}

result alcove::find_all_records(std::vector<record>& out_records) {
    auto hosts = util::open_hosts_for_reading();
    if (!hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    const auto tag_prefix = util::get_tag_prefix();

    std::string line;
    while (std::getline(hosts, line)) {
        auto tag_pos = line.find(tag_prefix);

        if (tag_pos == std::string::npos) {
            continue;
        }

        auto id = util::find_id(line);

        if (id < 0) {
            // this should never happen
            fmt::println("Record {} has an invalid id", line);
            continue;
        }

        record record;

        record.id = id;

        std::istringstream iss(line);
        if (!(iss >> record.ip >> record.domain)) {
            fmt::println("Record {} has an invalid format", line);
            continue;
        }

        out_records.push_back(record);
    }

    return result::SUCCESS;
}

// Utils

std::string alcove::get_alcove_error(result error) {
    switch (error) {
        case result::SUCCESS: return "no error";
        case result::HOSTS_READ_FAILED: return "failed to open hosts for reading";
        case result::HOSTS_WRITE_FAILED: return "failed to open hosts for writing";
        case result::RECORD_NOT_FOUND: return "record was not found";
        default: return "unknown alcove_result";
    }
}
