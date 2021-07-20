#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>


std::string VERSION = "0.1.0";


void PrintHelp() {
    std::cout <<
            "--version:     Print the version of the program\n"
            "-h, --help:    Show help\n";
    exit(1);
}

void ProcessArgs(int argc, char** argv) {
    const char* short_opts = "vh";
    const option long_opts[] = {
        {"version", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (opt == -1)
            break;
        switch (opt) {
            case 'v':
                std::cout << "Version " << VERSION << std::endl;
                break;
            case 'h':
            case '?':
            default:
                PrintHelp();
                break;
        }
    }
}

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    ProcessArgs(argc, argv);
    exit(1);
}
