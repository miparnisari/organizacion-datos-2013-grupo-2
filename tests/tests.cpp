#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

/*
 * @info https://code.google.com/p/googletest/
 */

int main(int argc,char** args)
{
	::testing::GTEST_FLAG(shuffle) = 1;
//	::testing::GTEST_FLAG(filter) = "*PPMC*";
	testing::InitGoogleTest(&argc, args);
	return RUN_ALL_TESTS();
}
