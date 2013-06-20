#include "../src/CapaLogica/Compresor/CompresorAritmetico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/CapaFisica/RegistroVariable.h"

// To use a test fixture, derive a class from testing::Test.
class TestCompresorAritmetico : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	CompresorAritmetico* compresor;
	ModeloProbabilistico* modelo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  std::vector<unsigned short> vector;
	  vector.push_back(65);
	  vector.push_back(66);
	  vector.push_back(67);
	  modelo = new ModeloProbabilistico();
	  modelo->inicializar_frecuencias_en_1(vector);
	  compresor = new CompresorAritmetico(modelo);
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  delete compresor;
  }

  // A helper function that some test uses.

};

TEST_F(TestCompresorAritmetico, ComprimirString)
{
	std::string string = "AABC";

	char* buffer = new char[2]();
	int tamanioComprimido = compresor->comprimir_todo(string.c_str(),string.size(),buffer);

	ASSERT_EQ(tamanioComprimido, 2);

	// 00100001 01000000
	ASSERT_EQ(buffer[0],33); //1000 0100
	ASSERT_EQ(buffer[1],64); //0000 0000

	delete buffer;
}
