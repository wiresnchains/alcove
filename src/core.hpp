#pragma once

#include <string>
#include <tuple>

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

private:
    alcove_result find_record(int index, int* out_line = nullptr) const;
    alcove_result find_free_index(int* out_index = nullptr) const;
};

std::string get_alcove_error(alcove_result result);

}
