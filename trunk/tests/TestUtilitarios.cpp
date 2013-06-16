/*
 * TestUtilitarios.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: maine
 */

#include "../src/Utilitarios/Utilitarios.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestUtilitarios : public testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(TestUtilitarios,Interpretar_byte)
{
	char byte = 0x80; // 1000 0000
	std::vector <bool> bools = utilitarios::interpretar_byte(byte);

	ASSERT_EQ(bools.size(), 8);
	ASSERT_EQ(bools[0],1);
	for (unsigned int i = 1; i < 8; i++)
		ASSERT_EQ(bools[i],0);


	bools.clear();

	byte = 0x40; // 0100 0000
	bools = utilitarios::interpretar_byte(byte);

	ASSERT_EQ(bools.size(), 8);
	ASSERT_EQ(bools[0],0);
	ASSERT_EQ(bools[1],1);
	for (unsigned int i = 2; i < 8; i++)
		ASSERT_EQ(bools[i],0);
}
