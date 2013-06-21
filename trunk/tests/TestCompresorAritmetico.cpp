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

	// 00100000+00000000
	ASSERT_EQ(buffer[0],32); //00100000
	ASSERT_EQ(buffer[1],0); //00000000

	delete buffer;
}

TEST_F(TestCompresorAritmetico, DescomprimirString)
{
	char buffer[2];
	buffer[0] = 32;
	buffer[1] = 0;

	char* bufferDescomprimido = new char[5]();
	ASSERT_TRUE(  compresor->descomprimir_todo((char*)&buffer,2,bufferDescomprimido,7,4)==RES_OK  );

	ASSERT_EQ(bufferDescomprimido[0],'A');
	ASSERT_EQ(bufferDescomprimido[1],'A');
	ASSERT_EQ(bufferDescomprimido[2],'B');
	ASSERT_EQ(bufferDescomprimido[3],'C');
	ASSERT_EQ(bufferDescomprimido[4], 0);

	delete bufferDescomprimido;
}

TEST_F(TestCompresorAritmetico, ComprimirRegistroVariableConUnCampo)
{
	delete compresor;
	compresor = new CompresorAritmetico();
	// Comprimo el registro
	RegistroVariable reg;
	std::string campo = "AABC";
	reg.agregar_campo(campo.c_str(), campo.size());
	RegistroVariable* reg_c = reg.comprimir(compresor);

	ASSERT_TRUE(reg_c != NULL);
	ASSERT_EQ(reg_c->get_cantidad_campos(), 2);

	char* campoUno = new char[4]();
	reg_c->recuperar_campo(campoUno,0);
	ASSERT_EQ(*campoUno,6);

	// Lo descomprimo
	RegistroVariable* reg_descomprimido = reg_c->descomprimir(compresor);
	ASSERT_EQ(reg_descomprimido->get_cantidad_campos(), 1);
	char* unicoCampo = new char[4]();
	reg_descomprimido->recuperar_campo(unicoCampo,0);

	ASSERT_EQ(unicoCampo[0],'A');
	ASSERT_EQ(unicoCampo[1],'B');
	ASSERT_EQ(unicoCampo[2],'B');
	ASSERT_EQ(unicoCampo[3],'C');

	delete[] campoUno;
	delete[] unicoCampo;
	delete reg_c;
}
