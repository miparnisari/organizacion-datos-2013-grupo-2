#include "../src/CapaLogica/Compresor/CompresorAritmetico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestCompresorAritmetico : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	CompresorAritmetico c;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
  }

  // A helper function that some test uses.

};


