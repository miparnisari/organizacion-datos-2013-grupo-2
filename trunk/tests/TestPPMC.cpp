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
	  const short orden = 3;
	  comp_ppmc = new PPMC(orden);
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  delete comp_ppmc;
  }

  // A helper function that some test uses.

};

TEST_F(TestPPMC, ComprimirString)
{
	string fuente = "TATATA";

	const int tamanio_buffer_comprimido = 50;
	char* buffer_comprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = comp_ppmc->comprimir_todo(fuente.c_str(),fuente.size(),buffer_comprimido);


	for (int i = 0; i < tamanio_buffer_comprimido; i++)
	{
		IMPRIMIR_MY_VARIABLE((int)buffer_comprimido[i]);
	}

	delete buffer_comprimido;

}

