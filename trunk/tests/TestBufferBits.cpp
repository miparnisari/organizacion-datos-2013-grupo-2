/*
 * TestBufferBits.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: martin
 */
#include "../src/CapaLogica/Compresor/BufferBits.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestBufferBits : public testing::Test {
 protected:
	// Declares the variables your tests want to use.

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestBufferBits,Buffer_bits)
{
	TamanioBitset const TBS= 10;
	BufferBits<TBS> bb();

}
