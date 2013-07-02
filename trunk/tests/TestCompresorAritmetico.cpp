#include "../src/CapaLogica/Compresor/Aritmetico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/CapaFisica/RegistroVariable.h"
#include "../src/CapaLogica/Parser/ParserCanciones.h"

// To use a test fixture, derive a class from testing::Test.
class TestCompresorAritmetico : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	Aritmetico* compresor;
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
	  compresor = new Aritmetico(modelo);
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

	char* buffer = new char[5]();
	int tamanioComprimido = compresor->comprimir_todo(string.c_str(),string.size(),buffer);

	ASSERT_EQ(tamanioComprimido, 5);

	ASSERT_EQ(buffer[0],32);
	ASSERT_EQ(buffer[1],-74);
	ASSERT_EQ(buffer[2],11);
	ASSERT_EQ(buffer[3],96);
	ASSERT_EQ(buffer[4],-112);

	delete[] buffer;
}


TEST_F(TestCompresorAritmetico, DescomprimirString)
{
	// El string comprimido ocupa 5 bytes
	const int tamanio_buffer = 5;
	char buffer[tamanio_buffer];
	buffer[0] = 32;
	buffer[1] = -74;
	buffer[2] = 11;
	buffer[3] = 96;
	buffer[4] = -112;

	const int tamanio_buffer_descomp = 4;
	char* bufferDescomprimido = new char[tamanio_buffer_descomp]();
	ASSERT_TRUE(  compresor->descomprimir_todo
			((char*)&buffer,
					tamanio_buffer,
					bufferDescomprimido,
					tamanio_buffer_descomp)==RES_OK  );

	ASSERT_EQ(bufferDescomprimido[0],'A');
	ASSERT_EQ(bufferDescomprimido[1],'A');
	ASSERT_EQ(bufferDescomprimido[2],'B');
	ASSERT_EQ(bufferDescomprimido[3],'C');

	delete[] bufferDescomprimido;
}

TEST_F(TestCompresorAritmetico, ComprimirRegistroVariableConUnCampo)
{
	delete compresor;
	compresor = new Aritmetico();
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
	RegistroVariable reg_descomprimido;
	reg_c->descomprimir(compresor,&reg_descomprimido);
	ASSERT_EQ(reg_descomprimido.get_cantidad_campos(), 1);
	char* unicoCampo = new char[4]();
	reg_descomprimido.recuperar_campo(unicoCampo,0);

	ASSERT_EQ(unicoCampo[0],'A');
	ASSERT_EQ(unicoCampo[1],'A');
	ASSERT_EQ(unicoCampo[2],'B');
	ASSERT_EQ(unicoCampo[3],'C');

	delete[] campoUno;
	delete[] unicoCampo;
	delete reg_c;
}


TEST_F(TestCompresorAritmetico,ComprimirCancion){

	Aritmetico ca;

	RegistroCancion registroOriginal;

	string archivo = "madonna;nicki minaj-2012-starships-english\n"
	"Red one \n"
	"Let’s go to the beach, each \n"
	"Let’s go get away \n"
	"They say, what they gonna say? \n"
	"Have a drink, clink, found the Bud Light \n"
	"Bad bitches like me, is hard to come by \n"
	"The Patrón, own, let’s go get it on \n"
	"The zone, own, yes I’m in the zone \n"
	"Is it two, three, leave a good tip \n"
	"I’ma blow all my money and don’t give two shits \n";

	ASSERT_EQ(registroOriginal.cargar(archivo.c_str(),archivo.size()),RES_OK);

	RegistroVariable* registroComprimido= registroOriginal.comprimir(&ca);
	ASSERT_TRUE(registroComprimido != NULL);

	RegistroCancion registroDescomprimido;

	registroComprimido->descomprimir( &ca , &registroDescomprimido );

	ASSERT_EQ(registroOriginal.get_letra(),registroDescomprimido.get_letra());
	ASSERT_EQ(registroOriginal.get_anio(),registroDescomprimido.get_anio());
	ASSERT_EQ(registroOriginal.get_cantidad_autores(),registroDescomprimido.get_cantidad_autores());

	delete registroComprimido;

}
