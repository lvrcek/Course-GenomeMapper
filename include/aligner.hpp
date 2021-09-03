#include <iostream>
#include <string>

namespace ivory_aligner {

enum AlignmentType { global, local, semiglobal };

enum Direction { up = 0, left = 1, diag = 2, stop = -1 };

int GlobalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap);

int LocalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap);

int SemiGlobalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap);

void PrintMatrix(int** matrix, const char* query, unsigned int query_len, const char* target, unsigned int target_len);

void PrintTraceback(Direction** traceback, const char* query, unsigned int query_len, const char* target, unsigned int target_len);

int Align(
    const char* query, unsigned int query_len,
    const char* target, unsigned int target_len,
    AlignmentType type,
    int match,
    int mismatch,
    int gap,
    std::string* cigar = nullptr,
    unsigned int* target_begin = nullptr);
}