#include "bioparser/fasta_parser.hpp"
#include "bioparser/fastq_parser.hpp"
#include <gtest/gtest.h>

// Test googletest integration
TEST(BasicTest, HandlesZeroInput) {
    EXPECT_STRNE("hello", "world");
    ASSERT_EQ(1, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}