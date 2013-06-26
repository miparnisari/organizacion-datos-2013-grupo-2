#include "../src/CapaLogica/Compresor/PPMC.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

class TestPPMC : public testing::Test {
 protected:
	PPMC* ppmc;
	unsigned short orden;


  virtual void SetUp() {
	  orden = 2;
	  ppmc = new PPMC(orden);
  }

  virtual void TearDown() {
	  delete ppmc;
  }
};

TEST_F(TestPPMC, ComprimirYDescomprimirStringTATATA)
{
	const string fuente = "TATATA";
	const unsigned int fuente_length = fuente.length();
	const int tamanio_buffer_comprimido = 20;

	char* buffer_comprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = ppmc->comprimir_todo(fuente.c_str(),fuente_length,buffer_comprimido);

	char* bufferDescomprimido= new char[ fuente_length ] ();

	int res = ppmc->descomprimir_todo( buffer_comprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] buffer_comprimido;
	delete[] bufferDescomprimido;
 }

TEST_F(TestPPMC,ComprimirYDescomprimirStringOrden2)
{
	const string fuente = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const unsigned int fuente_length = fuente.length();

	const int tamanio_buffer_comprimido = 100;
	char* bufferComprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = ppmc->comprimir_todo(fuente.c_str(),fuente_length,bufferComprimido);

	char* bufferDescomprimido= new char[ fuente_length ]();

	int res = ppmc->descomprimir_todo( bufferComprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] bufferComprimido;
	delete[] bufferDescomprimido;
}

TEST_F(TestPPMC,ComprimirYDescomprimirStringOrden3)
{
	const string fuente = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const unsigned int fuente_length = fuente.length();

	const int tamanio_buffer_comprimido = 100;
	char* bufferComprimido = new char[tamanio_buffer_comprimido]();

	PPMC* ppmc_o3 = new PPMC (3);

	int tam_comprimido = ppmc_o3->comprimir_todo(fuente.c_str(),fuente_length,bufferComprimido);

	char* bufferDescomprimido= new char[ fuente_length ]();

	int res = ppmc_o3->descomprimir_todo( bufferComprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] bufferComprimido;
	delete[] bufferDescomprimido;
	delete ppmc_o3;

}


TEST_F(TestPPMC,ComprimirYDescomprimirStringOrden4)
{
	const string fuente = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const unsigned int fuente_length = fuente.length();

	const int tamanio_buffer_comprimido = 100;
	char* bufferComprimido = new char[tamanio_buffer_comprimido]();

	PPMC* ppmc_o4 = new PPMC (4);

	int tam_comprimido = ppmc_o4->comprimir_todo(fuente.c_str(),fuente_length,bufferComprimido);

	char* bufferDescomprimido= new char[ fuente_length ]();

	int res = ppmc_o4->descomprimir_todo( bufferComprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] bufferComprimido;
	delete[] bufferDescomprimido;
	delete ppmc_o4;

}
