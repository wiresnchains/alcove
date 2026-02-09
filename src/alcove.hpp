#pragma once

#include <string>
#include <vector>

namespace alcove {
    enum class result {
        SUCCESS,
        HOSTS_READ_FAILED,
        HOSTS_WRITE_FAILED,
        RECORD_NOT_FOUND,
        INVALID_IP
    };

    struct record {
        int id;
        std::string ip;
        std::string domain;
    };

    result add_record(const std::string& ip, const std::string& domain, int* out_id = nullptr);
    result delete_record(int id);
    result clear_records();
    result find_all_records(std::vector<record>& out_records);
}
