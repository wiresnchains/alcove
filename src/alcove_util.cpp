#include "alcove_util.hpp"
#include <fmt/core.h>
#include <arpa/inet.h>

using namespace alcove;

constexpr auto MANAGED_RECORD_TAG = "alcove";

result util::find_next_id(int& out_id) {
    auto hosts = open_hosts_for_reading();
    if (!hosts.is_open()) {
        return result::HOSTS_READ_FAILED;
    }

    auto tag = fmt::format("#{}:", MANAGED_RECORD_TAG);
    int next_id = 0;

    std::string line;
    std::vector<int> taken_ids;
    while (std::getline(hosts, line)) {
        auto id = find_id(line);

        if (id < 0) {
            continue;
        }
        
        taken_ids.push_back(id);
    }

    std::sort(taken_ids.begin(), taken_ids.end());

    for (const int& taken_id : taken_ids) {
        if (next_id != taken_id) {
            break;
        }

        next_id++;
    }

    out_id = next_id;

    return result::SUCCESS;
}

std::vector<std::string> util::read_lines(std::ifstream& stream) {
    std::vector<std::string> lines;

    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    return lines;
}

int util::find_id(std::string record) {
    const auto tag_prefix = get_tag_prefix();
    auto tag_pos = record.find(tag_prefix);

    if (tag_pos == std::string::npos) {
        return -1;
    }

    int id;
    try {
        id = std::stoi(record.substr(tag_pos + tag_prefix.size()));
    }
    catch (const std::invalid_argument& e) {
        fmt::println("Invalid alcove tag in record: {}\n{}", record, e.what());
    }
    catch (const std::out_of_range& e) {
        fmt::println("Alcove tag id is too big in record: {}\n{}", record, e.what());
    }

    return id;
}

std::string util::format_tag(int id) {
    return fmt::format("#{}:{}", MANAGED_RECORD_TAG, id);
}

std::string util::get_tag_prefix() {
    return fmt::format("#{}:", MANAGED_RECORD_TAG);
}

constexpr auto HOSTS_PATH =
#if defined(_WIN32)
"C:\\Windows\\System32\\drivers\\etc\\hosts"
#elif defined(__APPLE__) || defined(__linux__)
"/etc/hosts"
#endif
;

std::ifstream util::open_hosts_for_reading() {
    return std::ifstream(HOSTS_PATH);
}

std::ofstream util::open_hosts_for_writing(std::ios_base::openmode mode) {
    return std::ofstream(HOSTS_PATH, mode);
}

bool util::is_valid_ipv4(const std::string& str) {
    sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &sa.sin_addr) == 1;
}

bool util::is_valid_ipv6(const std::string& str) {
    sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &sa.sin6_addr) == 1;
}
