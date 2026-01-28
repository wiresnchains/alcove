#pragma once

#include <string>
#include <vector>

namespace alcove {

enum class result {
    SUCCESS,
    HOSTS_READ_FAILED,
    HOSTS_WRITE_FAILED,
    RECORD_NOT_FOUND
};

struct record_entry {
    int idx;
    std::string ip;
    std::string domain;
};

result add_record(const std::string& ip, const std::string& domain_mask, int* out_idx = nullptr);
result delete_record(int idx);
result find_all_records(std::vector<record_entry>* out_record_entries);
result clear_records();
std::string get_alcove_error(result error);

}
