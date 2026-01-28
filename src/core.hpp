#pragma once

#include <string>
#include <vector>

namespace alcove {

enum class alcove_result {
    SUCCESS,
    COULD_NOT_OPEN_HOSTS,
    RECORD_NOT_FOUND
};

class Core {
public:
    alcove_result add_record(const std::string& ip, const std::string& domain_mask, int* out_index = nullptr) const;
    alcove_result delete_record(int index) const;
};

std::string get_alcove_error(alcove_result result);

}
