#include "alcove.hpp"
#include "alcove_util.hpp"
#include <cxxopts.hpp>
#include <fmt/base.h>

constexpr auto BUILD_MODE =
#if defined(_RELEASE)
"release"
#elif defined(_DEBUG)
"debug"
#endif
;

void print_logo() {
    fmt::println(R"(    ___    __                   
   /   |  / /________ _   _____ 
  / /| | / / ___/ __ \ | / / _ \
 / ___ |/ / /__/ /_/ / |/ /  __/
/_/  |_/_/\___/\____/|___/\___/
   
       alcove v1.1.1 {}
Mask local IPs with custom domains
for seamless local development
)", BUILD_MODE);
}

int _main(int arg_count, char* args[]) {
    cxxopts::Options options("alcove", "Mask local IPs with custom domains for seamless local development.");

    options.add_options()
        ("h,help", "Displays help menu")
        ("v,version", "Displays version")
        ("l,list", "List all records")
        ("m,managed", "List all managed records")
        ("a,add", "Add a new record", cxxopts::value<std::vector<std::string>>())
        ("d,delete", "Delete a record", cxxopts::value<int>())
        ("c,clear", "Delete all managed record");

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
    
    if (result.count("list")) {
        std::vector<alcove::record> records;
        if (auto result = alcove::find_all_records(records); result != alcove::result::SUCCESS) {
            fmt::println("Failed to find all records: {}", alcove::util::get_alcove_error(result));
            return 1;
        }

        for (const auto& record : records) {
            fmt::println("{} | {} -> {}", record.id, record.ip, record.domain);
        }

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

            if (auto result = alcove::add_record(ip, domain_mask, &record_index); result != alcove::result::SUCCESS) {
                fmt::println("Failed to add record: {}", alcove::util::get_alcove_error(result));
                return 1;
            }
        }

        return 0;
    }
    
    if (result.count("delete")) {
        auto record_index = result["delete"].as<int>();

        if (auto result = alcove::delete_record(record_index); result != alcove::result::SUCCESS) {
            fmt::println("Failed to delete record: {}", alcove::util::get_alcove_error(result));
            return 1;
        }

        return 0;
    }
    
    if (result.count("clear")) {
        if (auto result = alcove::clear_records(); result != alcove::result::SUCCESS) {
            fmt::println("Failed to clear records: {}", alcove::util::get_alcove_error(result));
            return 1;
        }

        return 0;
    }

    fmt::println("No options provided, use -h or --help to see a list of all available options");
    return 1;
}

int main(int arg_count, char* args[]) {
#if defined(_WIN32)
    alcove::util::init_winsocket();
#endif

    int res = _main(arg_count, args);

#if defined(_WIN32)
    alcove::util::cleanup_winsocket();
#endif

    return res;
}
