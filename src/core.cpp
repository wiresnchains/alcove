#include "core.hpp"

using namespace alcove;

alcove_result Core::add_record(const std::string& ip, const std::string& domain_mask, int* out_index) const {
    if (out_index != nullptr) {
        *out_index = 0;
    }

    return alcove_result::SUCCESS;
}

alcove_result Core::delete_record(int index) const {
    return alcove_result::SUCCESS;
}


std::string alcove::get_alcove_error(alcove_result result) {
    switch (result) {
        case alcove_result::SUCCESS: return "No error";
        case alcove_result::COULD_NOT_OPEN_HOSTS: return "Failed to open hosts for writing";
        case alcove_result::RECORD_NOT_FOUND: return "Record was not found";
        default: return "Unknown alcove_result";
    }
}
