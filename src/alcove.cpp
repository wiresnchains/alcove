#include "alcove.hpp"
#include <fmt/format.h>
#include <fstream>

using namespace alcove;

constexpr char tag[] = "alcove";

result find_record(int idx, int* out_line = nullptr);
result find_free_idx(int* out_idx = nullptr);
std::string format_tag(int idx);

result alcove::add_record(const std::string& ip, const std::string& domain_mask, int* out_idx) {
    int idx;
    auto result = find_free_idx(&idx);

    if (result != result::SUCCESS) {
        return result;
    }

    std::ofstream hosts("/etc/hosts", std::ios::app);
    if (!hosts.is_open()) {
        return result::COULD_NOT_OPEN_HOSTS;
    }

    hosts << fmt::format("{} {} {}\n", ip, domain_mask, format_tag(idx));

    if (out_idx != nullptr) {
        *out_idx = idx;
    }

    return result::SUCCESS;
}

result alcove::delete_record(int idx) {
    int record_line;
    auto result = find_record(idx, &record_line);
    if (result != result::SUCCESS) {
        return result;
    }

    std::ifstream in_hosts("/etc/hosts");
    if (!in_hosts.is_open()) {
        return result::COULD_NOT_OPEN_HOSTS;
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
        return result::COULD_NOT_OPEN_HOSTS;
    }

    for (const auto& line : lines) {
        out_hosts << line << '\n';
    }

    return result::SUCCESS;
}

std::string alcove::get_alcove_error(result error) {
    switch (error) {
        case result::SUCCESS: return "no error";
        case result::COULD_NOT_OPEN_HOSTS: return "failed to open hosts for writing";
        case result::RECORD_NOT_FOUND: return "record was not found";
        default: return "unknown alcove_result";
    }
}

result find_record(int idx, int* out_line) {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return result::COULD_NOT_OPEN_HOSTS;
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
        return result::COULD_NOT_OPEN_HOSTS;
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
