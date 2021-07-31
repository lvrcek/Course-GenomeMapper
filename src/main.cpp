#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "bioparser/fasta_parser.hpp"
#include "bioparser/fastq_parser.hpp"


std::string VERSION = "0.1.0";


struct FASTASequence {
  public:
    std::string name;
    std::string data;

    FASTASequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len) {
        this->name = name;
        this->data = data;
        // std::cout << this->name << std::endl;
        // std::cout << this->data << std::endl;
    }
};

struct FASTQSequence {
  public:
    std::string name;
    std::string data;
    std::string quality;

    FASTQSequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len,
                  const char* quality, std::uint32_t quality_len) {
        this->name = name;
        this->data = data;
        this->quality = quality;
    }
};

void PrintHelp() {
    std::cout <<
            "-v, --version: Print the version of the program\n"
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
                exit(0);
            case 'h':
            case '?':
            default:
                PrintHelp();
                break;
        }
    }

    if (optind >= argc) {
        std::cerr << "Error: Missing refernce and sequence files" << std::endl;
        exit(1);
    }

    //optind++;
    std::cout << argc << " " << *argv << " " << optind << std::endl;
    std::cout << "Found reference file: " << argv[optind] << std::endl;
    std::string fasta_path = argv[optind];
    optind++;

    const char *ss = "abc";
    const char *aa = "lololol";
    // FASTASequence fasta(ss, 3, aa, 5);
    // assert FASTA

    auto p = bioparser::Parser<FASTASequence>::Create<bioparser::FastaParser>(fasta_path);
    auto s = p->Parse(-1);
    FASTASequence fasta = *s[0];
    std::cout << fasta.name.substr(0, 200) << std::endl;

    if (optind >= argc) {
        std::cerr << "Error: Missing sequence file(s)" << std::endl;
        exit(1);
    }

    std::vector<int> v;
    for (int i = optind; i < argc; i++) {
        std::cout << "Found sequence file: " << argv[i] << std::endl;
        // assert FASTA/Q
    }
    // Parse each file
    // Append vector with sequences
    // Each sequence should be FASTQ record - probably solved in biosoup / bioparser

}

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    ProcessArgs(argc, argv);
    exit(1);
}
