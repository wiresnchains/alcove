#include <cxxopts.hpp>
#include <fmt/core.h>
#include "alcove.hpp"

void print_logo();

int main(int arg_count, char* args[]) {
    cxxopts::Options options("alcove", "Mask local IPs with custom domains for seamless local development.");

    options.add_options()
        ("h,help", "Displays help menu")
        ("v,version", "Displays version")
        ("l,list", "List all active tunnels")
        ("a,add", "Add a new tunnel", cxxopts::value<std::vector<std::string>>())
        ("d,delete", "Delete a tunnel", cxxopts::value<int>())
        ("s,shutdown", "Delete all active tunnels");

    cxxopts::ParseResult result;

    try {
        result = options.parse(arg_count, args);
    }
    catch (const cxxopts::exceptions::exception& e) {
        fmt::println("{}", e.what());
        return 1;
    }

    if (result.count("help")) {
        fmt::println("{}", options.help());
        return 0;
    }
    
    if (result.count("version")) {
        print_logo();
        return 0;
    }

    if (result.count("add")) {
        auto records = result["add"].as<std::vector<std::string>>();
        auto record_count = records.size();

        if (record_count % 2 != 0) {
            fmt::println("Please provide pairs of ip and domain mask");
            return 1;
        }

        for (int i = 0; i + 1 < record_count; i+= 2) {
            std::string ip = records[i];
            std::string domain_mask = records[i + 1];

            int record_index;
            auto result = alcove::add_record(ip, domain_mask, &record_index);

            if (result != alcove::result::SUCCESS) {
                fmt::println("Failed to add record: {}", alcove::get_alcove_error(result));
                return 1;
            }

            fmt::println("Added record {} ({} -> {})", record_index, ip, domain_mask);
        }

        return 0;
    }
    
    if (result.count("delete")) {
        auto record_index = result["delete"].as<int>();
        auto result = alcove::delete_record(record_index);

        if (result != alcove::result::SUCCESS) {
            fmt::println("Failed to delete record: {}", alcove::get_alcove_error(result));
            return 1;
        }

        fmt::println("Deleted record {}", record_index);

        return 0;
    }

    fmt::println("No options provided, use -h or --help to see a list of all available options");
    return 1;
}

void print_logo() {
    fmt::println(R"(    ___    __                   
   /   |  / /________ _   _____ 
  / /| | / / ___/ __ \ | / / _ \
 / ___ |/ / /__/ /_/ / |/ /  __/
/_/  |_/_/\___/\____/|___/\___/
   
          alcove v1.0
Mask local IPs with custom domains
for seamless local development
)");
}
