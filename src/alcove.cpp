#include "alcove.hpp"
#include <fmt/format.h>
#include <fstream>
#include <sstream>

using namespace alcove;

constexpr char tag[] = "alcove";

result find_record(int idx, int* out_line);
result find_free_idx(int* out_idx);
std::string format_tag(int idx);

result alcove::add_record(const std::string& ip, const std::string& domain_mask, int* out_idx) {
    int idx;

    if (auto result = find_free_idx(&idx); result != result::SUCCESS) {
        return result;
    }

    std::ofstream hosts("/etc/hosts", std::ios::app);
    if (!hosts.is_open()) {
        return result::HOSTS_WRITE_FAILED;
    }

    hosts << fmt::format("{} {} {}\n", ip, domain_mask, format_tag(idx));

    if (out_idx != nullptr) {
        *out_idx = idx;
    }

    return result::SUCCESS;
}

result alcove::delete_record(int idx) {
    int record_line;
    
    if (auto result = find_record(idx, &record_line); result != result::SUCCESS) {
        return result;
    }

    std::ifstream in_hosts("/etc/hosts");
    if (!in_hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in_hosts, line)) {
        lines.push_back(line);
    }
    in_hosts.close();

    lines.erase(lines.begin() + record_line);

    std::ofstream out_hosts("/etc/hosts", std::ios::trunc);
    if (!out_hosts.is_open()) {
        return result::HOSTS_WRITE_FAILED;
    }

    for (const auto& line : lines) {
        out_hosts << line << '\n';
    }

    return result::SUCCESS;
}

result alcove::find_all_records(std::vector<record_entry>* out_record_entries) {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    if (out_record_entries != nullptr) {
        out_record_entries->clear();
    }

    std::string line;
    while (std::getline(hosts, line)) {
        auto tag_pos = line.find(fmt::format("#{}:", tag));
        if (tag_pos == std::string::npos) {
            continue;
        }

        record_entry record;

        try {
            record.idx = std::stoi(line.substr(tag_pos + 1 + sizeof(tag) / sizeof(char)));
        } catch (...) {
            // ignore invalid comments
        }

        std::istringstream iss(line);

        if (!(iss >> record.ip >> record.domain)) {
            continue;
        }

        if (out_record_entries != nullptr) {
            out_record_entries->push_back(record);
        }
    }

    return result::SUCCESS;
}

result alcove::clear_records() {
    std::vector<record_entry> records;
    if (auto result = find_all_records(&records); result != result::SUCCESS) {
        return result;
    }
    
    for (const auto& record : records) {
        if (auto result = delete_record(record.idx); result != result::SUCCESS) {
            return result;
        }
    }

    return result::SUCCESS;
}

std::string alcove::get_alcove_error(result error) {
    switch (error) {
        case result::SUCCESS: return "no error";
        case result::HOSTS_READ_FAILED: return "failed to open hosts for reading";
        case result::HOSTS_WRITE_FAILED: return "failed to open hosts for writing";
        case result::RECORD_NOT_FOUND: return "record was not found";
        default: return "unknown alcove_result";
    }
}

result find_record(int idx, int* out_line) {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    std::string line;
    int line_number = 0;

    auto record_tag = format_tag(idx);

    while (std::getline(hosts, line)) {
        if (line.find(record_tag) != std::string::npos) {
            if (out_line != nullptr) {
                *out_line = line_number;
            }

            return result::SUCCESS;
        }

        line_number++;
    }

    return result::RECORD_NOT_FOUND;
}

result find_free_idx(int* out_idx) {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    int next_idx = 0;
    std::string line;

    while (std::getline(hosts, line)) {
        auto pos = line.find("#alcove:");

        if (pos == std::string::npos) {
            continue;
        }

        try {
            int idx = std::stoi(line.substr(pos + 1 + sizeof(tag) / sizeof(char)));

            if (idx >= next_idx) {
                next_idx = idx + 1;
            }
        } catch (...) {
            // ignore invalid comments
        }
    }

    if (out_idx != nullptr) {
        *out_idx = next_idx;
    }

    return result::SUCCESS;
}

std::string format_tag(int index) {
    return fmt::format("#{}:{}", tag, index);
}
