#pragma once

#include <string>

namespace alcove {

enum class result {
    SUCCESS,
    COULD_NOT_OPEN_HOSTS,
    RECORD_NOT_FOUND
};

result add_record(const std::string& ip, const std::string& domain_mask, int* out_idx = nullptr);
result delete_record(int idx);
std::string get_alcove_error(result error);

}
