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

TEST_F(TestPPMC, ComprimirYDescomprimirString)
{
	const string fuente = "TATATA";
	const unsigned int fuente_length = fuente.length();
	const int tamanio_buffer_comprimido = 20;

	char* buffer_comprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = ppmc->comprimir_todo(fuente.c_str(),fuente_length,buffer_comprimido);
	IMPRIMIR_MY_VARIABLE(tam_comprimido);

	for(int i=0;i<tam_comprimido;i++)
		IMPRIMIR_MY_VARIABLE( (int)buffer_comprimido[i] );

	char * bufferDescomprimido= new char[ fuente_length ] ();

	int res = ppmc->descomprimir_todo( buffer_comprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] buffer_comprimido;
	delete[] bufferDescomprimido;
 }

TEST_F(TestPPMC,DISABLED_ComprimirStringIterativamente){

	string linea="TATATA";
	const Uint TAMANIO_BUFFER_COMPRESION= 128;
	char bufferCompresion[TAMANIO_BUFFER_COMPRESION];
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> bufferBits;
	Uint indiceBufferCompresion= 0;
	string nombreContexto= "0";
	int numeroOrden= 0;

	for( Uint i=0;i<linea.length();i++ ){

		vector<bool> bitsEmitir;
		Uint simbolo= (Uint)linea.at(i);
		ppmc->comprimir_un_caracter(numeroOrden,i,simbolo,nombreContexto,bufferBits,bitsEmitir,bufferCompresion,indiceBufferCompresion,false);

		IMPRIMIR_MY_VARIABLE(nombreContexto);
		IMPRIMIR_MY_VARIABLE(numeroOrden);

	}

}

TEST_F(TestPPMC,ComprimirYDescomprimirStringLargo)
{
	const string fuente = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const unsigned int fuente_length = fuente.length();

	const int tamanio_buffer_comprimido = 100;
	char* bufferComprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = ppmc->comprimir_todo(fuente.c_str(),fuente_length,bufferComprimido);

	for(int i=0;i<tam_comprimido;i++)
		IMPRIMIR_MY_VARIABLE( (int)bufferComprimido[i] );

	char* bufferDescomprimido= new char[ fuente_length ]();

	int res = ppmc->descomprimir_todo( bufferComprimido,tam_comprimido,bufferDescomprimido,fuente_length );

	for(unsigned int i= 0;i<fuente_length;i++)
		IMPRIMIR_MY_VARIABLE( (char)bufferDescomprimido[i] );

	for (unsigned int i=0; i<fuente_length;i++)
		ASSERT_EQ(bufferDescomprimido[i],fuente[i]);

	ASSERT_EQ(res,RES_OK);

	delete[] bufferComprimido;
	delete[] bufferDescomprimido;
}
