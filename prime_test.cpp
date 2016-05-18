#include <limits.h>
#include "prime.hpp"
#include "gtest/gtest.h"

// Tests negative input.
TEST(IsPrimeTest, Negative) 
{
	{
		PrimeCandidate Candidate(-1);
		EXPECT_FALSE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(-2);
		EXPECT_FALSE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(INT_MIN);
		EXPECT_FALSE(Candidate.IsPrime());
	}

}

// Tests some trivial cases.
TEST(IsPrimeTest, Trivial) 
{
	{
		PrimeCandidate Candidate(0);
		EXPECT_FALSE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(1);
		EXPECT_FALSE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(2);
		EXPECT_TRUE(Candidate.IsPrime());
	}
}

// Tests positive input.
TEST(IsPrimeTest, Positive) 
{
	{
		PrimeCandidate Candidate(5);
		EXPECT_TRUE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(10);
		EXPECT_FALSE(Candidate.IsPrime());
	}

	{
		PrimeCandidate Candidate(INT_MAX);
		EXPECT_TRUE(Candidate.IsPrime());
	}
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
