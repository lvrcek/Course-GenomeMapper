// Copyright (c) 2021 Lovro Vrcek

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "bioparser/fasta_parser.hpp"
#include "bioparser/fastq_parser.hpp"

#include "ivory_config.hpp"
#include "aligner.hpp"

// #define VERSION "0.2.0"


struct Sequence {
 public:
    std::string name;
    std::string data;
    std::string quality;

    Sequence(const char* name, std::uint32_t name_len,
             const char* data, std::uint32_t data_len) :
                  name(name, name_len),
                  data(data, data_len) {}

    Sequence(const char* name, std::uint32_t name_len,
             const char* data, std::uint32_t data_len,
             const char* quality, std::uint32_t quality_len) :
                  name(name, name_len),
                  data(data, data_len),
                  quality(quality, quality_len) {}
};

void PrintStatistics(const std::vector<std::unique_ptr<Sequence>>& sequences, int mode) {  // NOLINT
    int num_sequences = sequences.size();
    int total_len = 0, n50_sum = 0;
    int min_len, max_len, mean_len, n50;
    std::vector<int> lengths;

    for (auto it = sequences.begin(); it != sequences.end(); it++) {
        lengths.push_back((*it)->data.size());
        total_len += (*it)->data.size();
    }

    std::sort(lengths.begin(), lengths.end(), [](int a, int b) {return a > b;});
    max_len = lengths[0];
    min_len = lengths[num_sequences-1];
    mean_len = total_len / num_sequences;

    for (int i = 0; i < num_sequences; i++) {
        n50_sum += lengths[i];
        if (n50_sum >= total_len * 0.5) {
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
            "usage: ivory_mapper [options ...] <reference> <fragments> [<fragments> ...]\n"  // NOLINT
            "\n"
            "  <reference>\n"
            "    input file containing reference in FASTA format (can be compressed with gzip)\n"  // NOLINT
            "  <fragments>\n"
            "    input file containing fragments in FASTA/Q format (can be compressed with gzip)\n"  // NOLINT
            "  options:\n"
            "    -v, --version\n"
            "      print the version of the program\n"
            "    -h, --help\n"
            "      show help\n";
}

void ProcessArgs(int argc, char** argv,
                 std::vector<std::unique_ptr<Sequence>>* reference,
                 std::vector<std::unique_ptr<Sequence>>* fragments) {
    const char* short_opts = "vh";
    const option long_opts[] = {
        {"version", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);  // NOLINT
        if (opt == -1)
            break;
        switch (opt) {
            case 'v':
                std::cout << "v" << VERSION << std::endl;
                exit(0);
            case 'h':
                PrintHelp();
                exit(0);
            case '?':
            default:
                PrintHelp();
                exit(1);
        }
    }

    if (optind >= argc) {
        std::cerr << "Error: Missing refernce and sequence files" << std::endl;
        PrintHelp();
        exit(1);
    }

    auto ends_with = [](const std::string &a, const std::string &b) {
        if (a.size() >= b.size())
            return (a.compare(a.size() - b.size(), b.size(), b) == 0);
        else
            return false;
    };

    std::string path = argv[optind++];
    if (!(ends_with(path, ".fasta") || ends_with(path, ".fasta.gz") ||
          ends_with(path, ".fna") || ends_with(path, ".fna.gz") ||
          ends_with(path, ".fa") || ends_with(path, ".fa.gz"))) {
        std::cerr << "Error: Unsupported file type" << std::endl;
        PrintHelp();
        exit(1);
        }

    auto p = bioparser::Parser<Sequence>::Create<bioparser::FastaParser>(path);
    *reference = p->Parse(-1);

    if (optind >= argc) {
        std::cerr << "Error: Missing sequence file(s)" << std::endl;
        PrintHelp();
        exit(1);
    }

    for (int i = optind; i < argc; i++) {
        std::string path = argv[i];
        if (!(ends_with(path, ".fasta") || ends_with(path, ".fasta.gz") ||
              ends_with(path, ".fastq") || ends_with(path, ".fastq.gz") ||
              ends_with(path, ".fna") || ends_with(path, ".fna.gz") ||
              ends_with(path, ".fa") || ends_with(path, ".fa.gz") ||
              ends_with(path, ".fq") || ends_with(path, ".fq.gz"))) {
            std::cerr << "Error: Unsupported file type" << std::endl;
            PrintHelp();
            exit(1);
        }

        auto p = bioparser::Parser<Sequence>::Create<bioparser::FastaParser>(path);  // NOLINT
        auto s = p->Parse(-1);
        fragments->insert(fragments->end(),
                          std::make_move_iterator(s.begin()),
                          std::make_move_iterator(s.end()));
    }
}

void TestAligner() {
    int score;
    const char *query, *target;
    unsigned int query_len, target_len;
    std::string cigar;
    unsigned int target_begin;

    query = "GATTACA";
    query_len = std::string(query).size();
    target = "GCATGCU";
    target_len = std::string(target).size();
    std::cout << "Query sequence: " << query << std::endl << "Target sequence: " << target << std::endl;
    score = ivory_aligner::Align(query, query_len, target, target_len, ivory_aligner::global, 1, -1, -1, &cigar, &target_begin);
    std::cout << "Alignment score: " << score << std::endl;
    std::cout << "CIGAR string: " << cigar << std::endl;
    std::cout << "Target begin: " << target_begin << std::endl  << std::endl;

    query = "ACCTAAGG";
    query_len = std::string(query).size();
    target = "GGCTCAATCA";
    target_len = std::string(target).size();
    std::cout << "Query sequence: " << query << std::endl << "Target sequence: " << target << std::endl;
    score = ivory_aligner::Align(query, query_len, target, target_len, ivory_aligner::local, 2, -1, -2, &cigar, &target_begin);
    std::cout << "Alignment score: " << score << std::endl;
    std::cout << "CIGAR string: " << cigar << std::endl;
    std::cout << "Target begin: " << target_begin << std::endl  << std::endl;

    query = "CGATAAA";
    query_len = std::string(query).size();
    target = "ACTCCGAT";
    target_len = std::string(target).size();
    std::cout << "Query sequence: " << query << std::endl << "Target sequence: " << target << std::endl;
    score = ivory_aligner::Align(query, query_len, target, target_len, ivory_aligner::semiglobal, 1, -1, -1, &cigar, &target_begin);
    std::cout << "Alignment score: " << score << std::endl;
    std::cout << "CIGAR string: " << cigar << std::endl;
    std::cout << "Target begin: " << target_begin << std::endl  << std::endl;
}

void TestMax() {
    int a = 5, b = 5, c = 5;
    std::vector<std::pair<int, ivory_aligner::Direction>> v;
    v.push_back(std::make_pair(a, ivory_aligner::up));
    v.push_back(std::make_pair(b, ivory_aligner::left));
    v.push_back(std::make_pair(c, ivory_aligner::diag));
    auto el = std::max_element(v.begin(), v.end());
    std::cout << el->first << " " << el->second << std::endl;
}

int main(int argc, char **argv) {
    std::vector<std::unique_ptr<Sequence>> reference, fragments;
    // TestMax();
    // return 0;
    TestAligner();
    return 0;
    ProcessArgs(argc, argv, &reference, &fragments);
    PrintStatistics(reference, 1);
    PrintStatistics(fragments, 2);
    return 0;
}
