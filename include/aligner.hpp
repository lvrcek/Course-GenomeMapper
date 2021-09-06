// Copyright (c) 2021 Lovro Vrcek

#ifndef INCLUDE_ALIGNER_HPP_
#define INCLUDE_ALIGNER_HPP_

#include <iostream>
#include <string>

namespace ivory_aligner {

enum AlignmentType { global, local, semiglobal };

enum Direction { up = 0, left = 1, diag = 2, stop = 3 };

int GlobalAlignment(
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len,
        int match,
        int mismatch,
        int gap,
        std::string* cigar,
        unsigned int* target_begin,
        bool matrix_print);

int LocalAlignment(
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len,
        int match,
        int mismatch,
        int gap,
        std::string* cigar,
        unsigned int* target_begin,
        bool matrix_print);

int SemiGlobalAlignment(
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len,
        int match,
        int mismatch,
        int gap,
        std::string* cigar,
        unsigned int* target_begin,
        bool matrix_print);

void PrintMatrix(
        int** matrix,
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len);

void PrintTraceback(
        Direction** traceback,
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len);

std::string GetCigar(Direction** traceback, unsigned int query_end,
                     unsigned int target_end);

unsigned int GetTargetBegin(Direction** traceback, unsigned int query_end,
                            unsigned int target_end);

int Align(
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len,
        AlignmentType type,
        int match,
        int mismatch,
        int gap,
        std::string* cigar = nullptr,
        unsigned int* target_begin = nullptr,
        bool matrix_print = false);

}  // namespace ivory_aligner

#endif  // INCLUDE_ALIGNER_HPP_
