#include <cxxopts.hpp>
#include <fmt/core.h>

void print_logo();

int main(int arg_count, char* args[]) {
    cxxopts::Options options("alcove", "Mask local IPs with custom domains for seamless local development.");

    options.add_options()("h,help", "Displays help menu");
    options.add_options()("v,version", "Displays version");

    auto result = options.parse(arg_count, args);

    if (result.count("help")) {
        fmt::println("{}", options.help());
        return 0;
    }

    if (result.count("version")) {
        print_logo();
        return 0;
    }

    auto unmatched = result.unmatched();
    if (!unmatched.empty()) {
        for (const auto& unmatched_option : unmatched) {
            fmt::println("Unknown option '{}', use -h or --help to see a list of all available options", unmatched_option);
        }
    }
    else {
        fmt::println("No options provided, use -h or --help to see a list of all available options");
    }

    return 0;
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
