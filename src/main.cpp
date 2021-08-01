#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>

#include "bioparser/fasta_parser.hpp"
#include "bioparser/fastq_parser.hpp"


std::string VERSION = "0.1.0";


struct Sequence {
  public:
    std::string name;
    std::string data;
    std::string quality;

    Sequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len) {
        this->name = std::string(name, name_len);
        this->data = std::string(data, data_len);
    }

    Sequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len,
                  const char* quality, std::uint32_t quality_len) {
        this->name = std::string(name, name_len);
        this->data = std::string(data, data_len);
        this->quality = std::string(quality, quality_len);
    }

};

void PrintStatistics(std::vector<Sequence> sequences, int mode) {
    int num_sequences = sequences.size();
    int total_len = 0, n50 = 0;
    int min_len, max_len, mean_len;
    std::vector<int> lengths;

    for (auto it = sequences.begin(); it != sequences.end(); it++) {
        lengths.push_back(it->data.size());
        total_len += it->data.size();
    }

    std::sort(lengths.begin(), lengths.end(), [](int a, int b) {return a > b;});
    max_len = lengths[0];
    min_len = lengths[num_sequences-1];
    mean_len = total_len / num_sequences;

    for (int i = 0; i < num_sequences; i++) {
        n50 += lengths[i];
        if (n50 >= total_len / 2) {
            n50 = lengths[i];
            break;
        }
    }

    if (mode == 1)
        std::cerr << "\n--------------- Reference Statistics ---------------\n";
    else
        std::cerr << "\n--------------- Fragments Statistics ---------------\n";

    std::cerr <<
            "Number of sequences\t=\t" << num_sequences << std::endl <<
            "Total length\t\t=\t" << total_len << std::endl <<
            "Minimal length\t\t=\t" << min_len << std::endl <<
            "Maximal length\t\t=\t" << max_len << std::endl <<
            "Mean length\t\t=\t" << mean_len << std::endl <<
            "N50 value\t\t=\t" << n50 << std::endl;
}

void PrintHelp() {
    std::cout <<
            "usage: ivory_mapper [options ...] <reference> <fragments> [<fragments> ...]\n"
            "\n"
            "  <reference>\n"
            "    input file containing reference in FASTA format (can be compressed with gzip)\n"
            "  <fragments>\n"
            "    input file containing fragments in FASTA/Q format (can be compressed with gzip)\n"
            "  options:\n"
            "    -v, --version\n"
            "      print the version of the program\n"
            "    -h, --help\n"
            "      show help\n";

    exit(1);
}

void ProcessArgs(int argc, char** argv, std::vector<Sequence>* reference, std::vector<Sequence>* fragments) {
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
                std::cout << "v" << VERSION << std::endl;
                exit(0);
            case 'h':
            case '?':
            default:
                PrintHelp();
        }
    }

    if (optind >= argc) {
        std::cerr << "Error: Missing refernce and sequence files" << std::endl;
        PrintHelp();
    }

    auto p = bioparser::Parser<Sequence>::Create<bioparser::FastaParser>(argv[optind++]);
    auto s = p->Parse(-1);
    for (auto it = s.begin(); it != s.end(); it++) {
        (*reference).push_back(**it);
    }

    if (optind >= argc) {
        std::cerr << "Error: Missing sequence file(s)" << std::endl;
        PrintHelp();
    }

    for (int i = optind; i < argc; i++) {
        auto p = bioparser::Parser<Sequence>::Create<bioparser::FastaParser>(argv[i]);
        auto s = p->Parse(-1);
        for (auto it = s.begin(); it != s.end(); it++) {
            fragments->push_back(**it);
        }
    }
}

int main(int argc, char **argv) {
    std::vector<Sequence> reference, fragments;
    ProcessArgs(argc, argv, &reference, &fragments);
    PrintStatistics(reference, 1);
    PrintStatistics(fragments, 2);
    return 0;
}
