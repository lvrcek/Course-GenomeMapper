// Copyright (c) 2021 Lovro Vrcek

#ifndef INCLUDE_MINIMIZER_HPP_
#define INCLUDE_MINIMZER_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

namespace ivory {

std::tuple<unsigned int, unsigned int, bool> GetMinKmer();

std::vector<std::tuple<unsigned int, unsigned int, bool>> Minimize(
    const char* sequence, unsigned int sequence_len,
    unsigned int kmer_len,
    unsigned int window_len);

void Minimize(
    std::vector<const char*> sequence, std::vector<unsigned int> sequence_len,
    unsigned int kmer_len,
    unsigned int window_len,
    std::map<unsigned int, std::vector<std::tuple<unsigned int, bool , unsigned int>>>* lookup);

void Filter(double frequency);

// struct Overlap {int i;};

// std::vector<Overlap> Map(const char* sequence, unsigned int sequence_len);

}  // namespace ivory

#endif  // INCLUDE_MINIMIZER_HPP_