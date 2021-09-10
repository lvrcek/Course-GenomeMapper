// Copyright (c) 2021 Lovro Vrcek

#include "aligner.hpp"

#include "bioparser/fasta_parser.hpp"
#include "bioparser/fastq_parser.hpp"
#include "gtest/gtest.h"

// Test googletest integration
TEST(BasicTest, HandlesZeroInput) {
    EXPECT_STRNE("hello", "world");
    ASSERT_EQ(1, 1);
}

// Test global alignment without affine gaps
TEST(AlignerTest, GlobalAlignmentNoAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "GATTACA", 7, "GCATGCU", 7,
            ivory::global, 1, -1, -1, 0, 0,
            &cigar, &target_begin);

    EXPECT_EQ(score, 0);
    EXPECT_EQ(cigar, "1M1I1M1D4M");
    EXPECT_EQ(target_begin, 0);
}

// Test local alignment without affine gaps
TEST(AlignerTest, LocalAlignmentNoAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "ACCTAAGG", 8, "GGCTCAATCA", 10,
            ivory::local, 2, -1, -2, 0, 0,
            &cigar, &target_begin);

    EXPECT_EQ(score, 6);
    EXPECT_EQ(cigar, "2M1I2M");
    EXPECT_EQ(target_begin, 2);
}

// Test semi-global alignment without affine gaps
TEST(AlignerTest, SemiGlobalAlignmentNoAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "CGATAAA", 7, "ACTCCGAT", 8,
            ivory::semiglobal, 1, -1, -1, 0, 0,
            &cigar, &target_begin);

    EXPECT_EQ(score, 4);
    EXPECT_EQ(cigar, "4M");
    EXPECT_EQ(target_begin, 4);
}

// Test global alignment with affine gaps
TEST(AlignerTest, GlobalAlignmentAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "GATTACA", 7, "GCATGCU", 7,
            ivory::global, 1, -1, -1, -2, -1,
            &cigar, &target_begin);

    EXPECT_EQ(score, -1);
    EXPECT_EQ(cigar, "7M");
    EXPECT_EQ(target_begin, 0);
}

// Test local alignment with affine gaps
TEST(AlignerTest, LocalAlignmentAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "ACCTAAGG", 8, "GGCTCAATCA", 10,
            ivory::local, 2, -1, -2, -3, -1,
            &cigar, &target_begin);

    EXPECT_EQ(score, 5);
    EXPECT_EQ(cigar, "4M");
    EXPECT_EQ(target_begin, 2);
}

// Test semi-global alignment with affine gaps
TEST(AlignerTest, SemiGlobalAlignmentAffine) {
    std::string cigar;
    unsigned int target_begin;

    int score = ivory::Align(
            "CGATAAA", 7, "ACTCCGAT", 8,
            ivory::semiglobal, 1, -1, -1, -2, -1,
            &cigar, &target_begin);

    EXPECT_EQ(score, 4);
    EXPECT_EQ(cigar, "4M");
    EXPECT_EQ(target_begin, 4);
}
