#include "../src/CapaLogica/Compresor/CompresorAritmetico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/CapaFisica/RegistroVariable.h"

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

TEST_F(TestCompresorAritmetico, ComprimirRegistroVariable)
{
	RegistroVariable reg;

	std::string string = "AABC";
	reg.agregar_campo(string.c_str(),string.length());

	RegistroVariable* reg_comprimido = reg.comprimir(c);

	int cantidadCaracteres = -1;
	reg_comprimido->recuperar_campo((char*)&cantidadCaracteres,0);
	ASSERT_EQ(cantidadCaracteres, 8);

	char* buffer = new char[4]();
	reg_comprimido->recuperar_campo(buffer,1);


	delete buffer;
	delete reg_comprimido;


}
