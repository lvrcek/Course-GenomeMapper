#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>

#include "aligner.hpp"

namespace ivory_aligner {

int GlobalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap) {
    int** matrix = new int*[query_len + 1];
    for (int i = 0; i < query_len + 1; i++) {
        matrix[i] = new int[target_len + 1];
    }

    for (int j = 0; j < target_len + 1; j++)
        matrix[0][j] = mismatch * j;
    
    for (int i = 0; i < query_len  + 1; i++)
        matrix[i][0] = mismatch * i;

    for (int i = 1; i < query_len + 1; i++) {
        for (int j = 1; j < target_len + 1; j++) {
            int diag = matrix[i-1][j-1] + ((query[i-1] == target[j-1]) ? match : mismatch);
            int up = matrix[i-1][j] + gap;
            int left = matrix[i][j-1] + gap;
            matrix[i][j] = std::max({diag, up, left});
        }
    }
    printf("\n");
    PrintMatrix(matrix, query, query_len, target, target_len);
    return matrix[query_len][target_len];
}

int LocalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap) {
    int** matrix = new int*[query_len + 1];
    for (int i = 0; i < query_len + 1; i++) {
        matrix[i] = new int[target_len + 1];
    }

    for (int j = 0; j < target_len + 1; j++)
        matrix[0][j] = 0;
    
    for (int i = 0; i < query_len  + 1; i++)
        matrix[i][0] = 0;

    int score = 0;
    int end_query = 0, end_target = 0;

    for (int i = 1; i < query_len + 1; i++) {
        for (int j = 1; j < target_len + 1; j++) {
            int diag = matrix[i-1][j-1] + ((query[i-1] == target[j-1]) ? match : mismatch);
            int up = matrix[i-1][j] + gap;
            int left = matrix[i][j-1] + gap;
            matrix[i][j] = std::max({diag, up, left, 0});
            if (matrix[i][j] > score) {
                score = matrix[i][j];
                end_query = i;
                end_target = j;
            }
        }
    }
    printf("\n");
    PrintMatrix(matrix, query, query_len, target, target_len);
    return score;
}


int SemiGlobalAlignment(const char* query, unsigned int query_len,
                     const char* target, unsigned int target_len,
                     int match, int mismatch, int gap) {
    int** matrix = new int*[query_len + 1];
    for (int i = 0; i < query_len + 1; i++) {
        matrix[i] = new int[target_len + 1];
    }

    for (int j = 0; j < target_len + 1; j++)
        matrix[0][j] = 0;
    
    for (int i = 0; i < query_len  + 1; i++)
        matrix[i][0] = 0;

    int score = 0;
    int end_query = 0, end_target = 0;

    for (int i = 1; i < query_len + 1; i++) {
        for (int j = 1; j < target_len + 1; j++) {
            int diag = matrix[i-1][j-1] + ((query[i-1] == target[j-1]) ? match : mismatch);
            int up = matrix[i-1][j] + gap;
            int left = matrix[i][j-1] + gap;
            matrix[i][j] = std::max({diag, up, left});
            if ( (i == query_len || j == target_len) &&  matrix[i][j] > score) {
                score = matrix[i][j];
                end_query = i;
                end_target = j;
            }
        }
    }
    printf("\n");
    PrintMatrix(matrix, query, query_len, target, target_len);
    return score;
}

void PrintMatrix(int** matrix, const char * query, unsigned int query_len,
                 const char* target, unsigned int target_len) {
    printf("%4c%4c", ' ', ' ');
    for (int j = 0; j < target_len; j++)
        printf("%4c", target[j]);
    printf("\n");
    printf("%4c", ' ');
    for (int i = 0; i < query_len + 1; i++) {
        for (int j = 0; j < target_len + 1; j++)
            printf("%4d", matrix[i][j]);
        printf("\n");
        printf("%4c", query[i]);
    }
    printf("\n");
}

int Align(
        const char* query, unsigned int query_len,
        const char* target, unsigned int target_len,
        AlignmentType type,
        int match,
        int mismatch,
        int gap,
        std::string* cigar,
        unsigned int* target_begin) {
    
    int alignment_score;

    switch(type) {
        case global:
            alignment_score = GlobalAlignment(query, query_len, target, target_len, match, mismatch, gap);
            break;
        case local:
            alignment_score = LocalAlignment(query, query_len, target, target_len, match, mismatch, gap);
            break;
        case semiglobal:
            alignment_score = SemiGlobalAlignment(query, query_len, target, target_len, match, mismatch, gap);
            break;
    }

    return alignment_score;
}

}