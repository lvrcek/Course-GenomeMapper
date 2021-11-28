#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "aligner.hpp"


namespace ivory {

std::string ReverseComplement(std::string s) {
    std::string rc = "";
    for (int i = s.size() - 1; i >= 0; i--) {
        char c = s[i];
        switch (c) {
            case 'c':
            case 'C':
                rc += "G";
                break;
            case 'a':
            case 'A':
                rc += "T";
                break;
            case 't':
            case 'T':
                rc += "A";
                break;
            case 'g':
            case 'G':
                rc += "C";
                break;
        }
    }
    return rc;
}

unsigned int KmerHash(std::string s) {
    std::string new_s = "";
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        // CAGT -> 0123
        switch (c) {
            case 'c':
            case 'C':
                new_s += "0";
                break;
            case 'a':
            case 'A':
                new_s += "1";
                break;
            case 'g':
            case 'G':
                new_s += "2";
                break;
            case 't':
            case 'T':
                new_s += "3";
                break;
        }
    }
    
    unsigned int kmer = stoi(new_s);
    std::cout << s << " " << new_s << " " << kmer << std::endl;
    return kmer;
}

unsigned int KmerHashReverse(std::string s) {
    std::string new_s = "";
    for (int i = s.size() - 1; i >= 0; i--) {
        char c = s[i];
        switch (c) {
            case 'c':
            case 'C':
                new_s += "3";
                break;
            case 'a':
            case 'A':
                new_s += "2";
                break;
            case 'g':
            case 'G':
                new_s += "1";
                break;
            case 't':
            case 'T':
                new_s += "0";
                break;
        }
    }
    unsigned int kmer = stoi(new_s);
    std::cout << ReverseComplement(s) << " " << new_s << " " << kmer << std::endl;
    return kmer;
}

std::tuple<unsigned int, unsigned int, bool> GetMinKmer(
        const char* sequence, unsigned int sequence_len,
        unsigned int kmer_len, unsigned int window_len,
        unsigned int start) {
        // Isolate the window
        // Get all the kmers
        // Hash them
        // Get the smallest one
    std::vector<std::tuple<unsigned int, unsigned int, bool>> v;
    std::tuple<unsigned int, unsigned int, bool> t;
    unsigned int minimizer = stoi(std::string(kmer_len, '9'));
    for (int i = 0; i <= window_len - kmer_len; i++) {
        std::string kmer = "";
        for (int j = 0; j < kmer_len; j++) {
            kmer += sequence[start+i+j];
        }
        unsigned int kmer_h = KmerHash(kmer);
        unsigned int pos = start + i;
        // bool strand = true;
        if (kmer_h < minimizer) {
            t = {kmer_h, pos, true};
            minimizer = kmer_h;
        }
        unsigned int kmer_h_rc = KmerHashReverse(kmer);
        if (kmer_h_rc < minimizer) {
            t = {kmer_h_rc, pos, false};
            minimizer = kmer_h_rc;
        }
    }
    std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
    return t;
}

std::vector<std::tuple<unsigned int, unsigned int, bool>> Minimize(
        const char* sequence, unsigned int sequence_len,
        unsigned int kmer_len,
        unsigned int window_len) {
    // Slide the window over the string
    // For each window find all the kmers
    // Find unsigned int hash for each kmer
    // Take the smallest one for each window
    std::vector<std::tuple<unsigned int, unsigned int, bool>> V;
    for (int start=0; start <= sequence_len - window_len; start++) {
        std::tuple<unsigned int, unsigned int, bool> min_kmer = GetMinKmer(sequence, sequence_len,
                kmer_len, window_len, start);
        std::cout << "out" << std::endl;
        V.push_back(min_kmer);
    }
    
    
    // std::tuple<unsigned int, unsigned int, bool> t = {1 ,1, 1};
    // V.push_back(t);
    return V;

}

void Minimize(
        std::vector<const char*> sequence, std::vector<unsigned int> sequence_len,
        unsigned int kmer_len,
        unsigned int window_len) {
    return;

}

void Filter(double frequency) {
    return;
}

// struct Overlap {int i = 0;};

// std::vector<Overlap> Map(const char* sequence, unsigned int sequence_len) {
//     std::vector<Overlap> V;
//     return V;
// }

}  // namespace ivory

// int main() {
//     return 0;
// }