#pragma once

#include "alcove.hpp"
#include <string>
#include <fstream>

namespace alcove::util {
    result find_next_id(int& out_id);

    std::vector<std::string> read_lines(std::ifstream& stream);
    int find_id(std::string record);

    std::string format_tag(int id);
    std::string get_tag_prefix();
    
    std::ifstream open_hosts_for_reading();
    std::ofstream open_hosts_for_writing(std::ios_base::openmode mode = std::ios::app);
}
