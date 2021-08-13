// Copyright (c) 2021 Lovro Vrcek

#include "alignment.hpp"

#include <iostream>
#include <string>

namespace ivory_aligner {

void NeedlemanWunsch() {}

void SmithWaterman() {}

void SemiGlobal() {} 

int Align(
    const char* query, unsigned int query_len,
    const char* target, unsigned int target_len,
    AlignmentType type,
    int match,
    int mismatch,
    int gap,
    std::string* cigar,
    unsigned int* target_begin) { return 0; }
}
