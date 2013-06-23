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
	  const short orden = 2;
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

	const int tamanio_buffer_comprimido = 10;
	char* buffer_comprimido = new char[tamanio_buffer_comprimido]();

	int tam_comprimido = comp_ppmc->comprimir_todo(fuente.c_str(),fuente.size(),buffer_comprimido);

	ASSERT_EQ(buffer_comprimido[0],84);
	ASSERT_EQ(buffer_comprimido[1],-96);
	ASSERT_EQ(buffer_comprimido[2],-96);
	ASSERT_EQ(buffer_comprimido[3],0);
	delete buffer_comprimido;

}

TEST_F(TestPPMC, DescomprimirString)
{
	const int tamanio_buffer_comprimido = 4;
	char buffer_comprimido[tamanio_buffer_comprimido];
	buffer_comprimido[0] = 84;
	buffer_comprimido[1] = -96;
	buffer_comprimido[2] = -96;
	buffer_comprimido[3] = 0;

	const int tamanio_buffer_descomp = 6;
	char* bufferDescomprimido = new char[tamanio_buffer_descomp]();
	ASSERT_TRUE(  comp_ppmc->descomprimir_todo
			((char*)&buffer_comprimido,
					tamanio_buffer_comprimido,
					bufferDescomprimido,
					9,
					tamanio_buffer_descomp) ==RES_OK  );

	ASSERT_EQ(bufferDescomprimido[0],'T');
	ASSERT_EQ(bufferDescomprimido[1],'A');
	ASSERT_EQ(bufferDescomprimido[2],'T');
	ASSERT_EQ(bufferDescomprimido[3],'A');
	ASSERT_EQ(bufferDescomprimido[4],'T');
	ASSERT_EQ(bufferDescomprimido[5],'A');

	delete bufferDescomprimido;
}


TEST_F(TestPPMC,ComprimirStringIterativamente){

	string linea="TATATA";
	PPMC ppmc(2);
	const Uint TAMANIO_BUFFER_COMPRESION= 128;
	char bufferCompresion[TAMANIO_BUFFER_COMPRESION];
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> bufferBits;
	Uint indiceBufferCompresion= 0;
	string nombreContexto= "0";
	int numeroOrden= 0;


	for( Uint i=0;i<linea.length();i++ ){

		vector<bool> bitsEmitir;
		Uint simbolo= (Uint)linea.at(i);
		ppmc.comprimir_un_caracter(numeroOrden,i,simbolo,nombreContexto,bufferBits,bitsEmitir,bufferCompresion,indiceBufferCompresion,false);
		cout<<"ComprimirStringIterativamente: ..."<<endl;


		IMPRIMIR_MY_VARIABLE(nombreContexto);
		IMPRIMIR_MY_VARIABLE(numeroOrden);

	}

}
