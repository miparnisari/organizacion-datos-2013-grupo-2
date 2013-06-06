#include "../src/Utilitarios/Normalizador.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestNormalizador : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	Normalizador n;

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

TEST_F(TestNormalizador,Normalizar_string)
{
	string lineaLarga= "ésta-línea-se-ásemeja-múcho-A\nUNA CANCION\n";
	string lineaLargaNormalizada = n.normalizar_input(lineaLarga);
	ASSERT_TRUE(lineaLargaNormalizada == "esta-linea-se-asemeja-mucho-a\nuna cancion\n");
}
