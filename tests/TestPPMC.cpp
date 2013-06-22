#include "../src/CapaLogica/Compresor/PPMC.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestPPMC : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	PPMC* comp_ppmc;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  comp_ppmc = new PPMC(3);
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  delete comp_ppmc;
  }

  // A helper function that some test uses.

};

