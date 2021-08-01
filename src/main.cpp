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


struct FASTASequence {
  public:
    std::string name;
    std::string data;
    std::string quality;

    FASTASequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len) {
        this->name = std::string(name, name_len);
        this->data = std::string(data, data_len);
    }

    FASTASequence(const char* name, std::uint32_t name_len,
                  const char* data, std::uint32_t data_len,
                  const char* quality, std::uint32_t quality_len) {
        this->name = std::string(name, name_len);
        this->data = std::string(data, data_len);
        this->quality = std::string(quality, quality_len);
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
        this->name = std::string(name, name_len);
        this->data = std::string(data, data_len);
        this->quality = std::string(quality, quality_len);
    }
};

bool Key(int i, int j) { return i > j; }

void PrintStatistics(std::vector<FASTASequence> sequences) {
    // int num_sequences = sequences.size();
    // int min_len = sequences[0].data.size();
    // int max_len = sequences[0].data.size();
    // float mean_len = sequences[0].data.size();
    // int n = 1;
    // for (auto it = sequences.begin()++; it != sequences.end(); it++) {
    //     n++;
    //     if (it->data.size() > max_len)
    //         max_len = it->data.size();
    //     if (it->data.size() < min_len)
    //         min_len = it->data.size();
    //     mean_len = mean_len * (n - 1) / n + it->data.size() / n;
    // }

    int total_length = 0;
    std::vector<int> lengths;
    for (auto it = sequences.begin(); it != sequences.end(); it++) {
        lengths.push_back(it->data.size());
        total_length += it->data.size();
    }
    std::sort(lengths.begin(), lengths.end(), Key);
    int num_sequences = lengths.size();
    int max_len = lengths[0];
    int min_len = lengths[num_sequences-1];
    float mean_len = 0;
    int n50 = 0;
    for (int i = 0; i < num_sequences; i++) {
        mean_len += lengths[i];        
    }
    mean_len /= num_sequences;
    for (int i = 0; i < num_sequences; i++) {
        n50 += lengths[i];
        if (n50 >= total_length / 2) {
            n50 = lengths[i];
            break;
        }
    }



    std::cerr <<
            "---- Statistics -----" << std::endl <<
            "Num seqs\t=\t" << num_sequences << std::endl <<
            "Total len\t=\t" << total_length << std::endl <<
            "Min len\t\t=\t" << min_len << std::endl <<
            "Max len\t\t=\t" << max_len << std::endl <<
            "Mean len\t=\t" << int(mean_len) << std::endl <<
            "N50\t\t=\t" << n50 << std::endl;
    return;
}

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
    std::cout << fasta.name << std::endl;
    std::cout << fasta.data.substr(0, 50) << std::endl;
    std::vector<FASTASequence> v1;
    for (auto it = s.begin(); it != s.end(); it++) {
        v1.push_back(**it);
    }
    PrintStatistics(v1);

    if (optind >= argc) {
        std::cerr << "Error: Missing sequence file(s)" << std::endl;
        exit(1);
    }

    // std::vector<std::unique_ptr<FASTASequence>> v;
    std::vector<FASTASequence> v2;
    for (int i = optind; i < argc; i++) {
        std::cout << "Found sequence file: " << argv[i] << std::endl;
        std::string fastq_path = argv[i];
        auto p = bioparser::Parser<FASTASequence>::Create<bioparser::FastaParser>(fastq_path);
        auto s = p->Parse(-1);
        // v.insert(std::end(v), std::begin(s), std::end(s));
        for (auto it = s.begin(); it != s.end(); it++) {
            v2.push_back(**it);
        }
        FASTASequence fastq = *s[0];
        v2.push_back(fastq);
        std::cout << fastq.name << std::endl;
        std::cout << fastq.data.substr(0, 50) << std::endl;
        std::cout << s.size() << std::endl;
        // std::cout << fastq.quality.substr(0, 50)  << std::endl;
        // assert FASTA/Q
    }
    PrintStatistics(v2);
    // std::cout << v2.size() << std::endl;
    // Parse each file
    // Append vector with sequences
    // Each sequence should be FASTQ record - probably solved in biosoup / bioparser

}

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    ProcessArgs(argc, argv);
    exit(1);
}
