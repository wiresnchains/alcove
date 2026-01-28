#include "core.hpp"
#include <fstream>
#include <fmt/format.h>

using namespace alcove;

alcove_result Core::add_record(const std::string& ip, const std::string& domain_mask, int* out_index) const {
    if (out_index != nullptr) {
        auto result = find_free_index(out_index);

        if (result != alcove_result::SUCCESS) {
            return result;
        }
    }

    std::ofstream hosts("/etc/hosts", std::ios::app);
    if (!hosts.is_open()) {
        return alcove_result::COULD_NOT_OPEN_HOSTS;
    }

    hosts << fmt::format("{} {} #alcove:{}\n", ip, domain_mask, *out_index);

    return alcove_result::SUCCESS;
}

alcove_result Core::delete_record(int index) const {
    int record_line;
    auto result = find_record(index, &record_line);
    if (result != alcove_result::SUCCESS) {
        return result;
    }

    std::ifstream in_hosts("/etc/hosts");
    if (!in_hosts.is_open()) {
        return alcove_result::COULD_NOT_OPEN_HOSTS;
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
        return alcove_result::COULD_NOT_OPEN_HOSTS;
    }

    for (const auto& line : lines) {
        out_hosts << line << '\n';
    }

    return alcove_result::SUCCESS;
}

alcove_result Core::find_record(int index, int* out_line) const {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return alcove_result::COULD_NOT_OPEN_HOSTS;
    }

    std::string line;
    int line_number = 0;

    auto tag = fmt::format("#alcove:{}", index);

    while (std::getline(hosts, line)) {
        if (line.find(tag) != std::string::npos) {
            if (out_line != nullptr) {
                *out_line = line_number;
            }

            return alcove_result::SUCCESS;
        }

        line_number++;
    }

    return alcove_result::RECORD_NOT_FOUND;
}

alcove_result Core::find_free_index(int* out_index) const {
    std::ifstream hosts("/etc/hosts");
    if (!hosts.is_open()) {
        return alcove_result::COULD_NOT_OPEN_HOSTS;
    }

    int next_index = 0;
    std::string line;

    while (std::getline(hosts, line)) {
        auto pos = line.find("#alcove:");

        if (pos == std::string::npos) {
            continue;
        }

        try {
            int idx = std::stoi(line.substr(pos + 8));

            if (idx >= next_index) {
                next_index = idx + 1;
            }
        } catch (...) {
            // ignore invalid comments
        }
    }

    *out_index = next_index;

    return alcove_result::SUCCESS;
}

std::string alcove::get_alcove_error(alcove_result result) {
    switch (result) {
        case alcove_result::SUCCESS: return "no error";
        case alcove_result::COULD_NOT_OPEN_HOSTS: return "failed to open hosts for writing";
        case alcove_result::RECORD_NOT_FOUND: return "record was not found";
        default: return "unknown alcove_result";
    }
}
