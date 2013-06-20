#include "../src/CapaLogica/Compresor/CompresorAritmetico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/CapaFisica/RegistroVariable.h"

// To use a test fixture, derive a class from testing::Test.
class TestCompresorAritmetico : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	CompresorAritmetico* compresor;
	ModeloProbabilistico modelo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  std::vector<unsigned short> vector;
	  vector.push_back(65);
	  vector.push_back(66);
	  vector.push_back(67);
	  modelo.inicializar_frecuencias_en_1(vector);
	  compresor = new CompresorAritmetico(&modelo);
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  delete compresor;
  }

  // A helper function that some test uses.

};

TEST_F(TestCompresorAritmetico, ComprimirRegistroVariable)
{
	RegistroVariable reg;

	std::string string = "AABC";
	reg.agregar_campo(string.c_str(),string.length());

	RegistroVariable* reg_comprimido = reg.comprimir(compresor);

	TamanioCampos cantidadCaracteres = 0;
	reg_comprimido->recuperar_campo((char*)&cantidadCaracteres,0);
	ASSERT_EQ(cantidadCaracteres, 6);

	char* buffer = new char[4]();
	reg_comprimido->recuperar_campo(buffer,1);

	// 00100001 01000000
	ASSERT_EQ(buffer[0],33);
	ASSERT_EQ(buffer[1],64);

	delete buffer;
	delete reg_comprimido;
}
