#include "../src/CapaLogica/Indices/ArchivoListas.h"
#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestArchivoListas : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ArchivoListas archivo;
	ManejadorRegistrosVariables manejador;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	ASSERT_TRUE(archivo.crear("", "TestArchivoListas") == RES_OK);
	ASSERT_TRUE(archivo.abrir("", "TestArchivoListas") == RES_OK);
	ASSERT_TRUE(manejador.abrir_archivo("TestArchivoListas.dat") == RES_OK);
	ASSERT_TRUE(manejador.get_cantidad_registros_ocupados() == 0);
  }

  virtual void TearDown() {
	  ASSERT_EQ(manejador.eliminar_archivo("TestArchivoListas.dat"),RES_OK);
  }

  // A helper function that some test uses.

};

TEST_F(TestArchivoListas,Agregar)
{	RegistroVariable lista;
	int id = 23;

	//Agrego un elemento a la lista
	lista.agregar_campo((char*)&id,sizeof(int));
	//Agrego la lista al archivo
	ASSERT_TRUE(archivo.agregar(&lista) != RES_ERROR);
	ASSERT_TRUE(archivo.get_cantidad_listas() == 1);

	ASSERT_TRUE(manejador.get_cantidad_registros_ocupados() == 1);
	ASSERT_TRUE(manejador.get_registro_ocupado(&lista,0) > 0);
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	int idrecuperado;
	lista.recuperar_campo((char*)&idrecuperado,0);
	ASSERT_TRUE(idrecuperado == 23);
}

TEST_F(TestArchivoListas,Devolver)
{
	RegistroVariable lista;
	int id=23;

	//Agrego un elemento a la lista
	lista.agregar_campo((char*)&id, sizeof(int));
	//Agrego la lista al archivo
	archivo.agregar(&lista);

	//Obtengo la lista que se guardo en el archivo
	ASSERT_TRUE(archivo.get_cantidad_listas() == 1);
	archivo.devolver(&lista, 0);
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id,0);
	ASSERT_TRUE(id == 23);

}

TEST_F(TestArchivoListas,Reconstruir_listas)
{
	RegistroVariable lista1;
	int id=23;
	unsigned short* ref_listas = new unsigned short[1]();

	lista1.agregar_campo((char*)&id,sizeof(int));
	ASSERT_TRUE(archivo.agregar(&lista1) == 0);

	id=11;
	lista1.agregar_campo((char*)&id,sizeof(int));
	ASSERT_TRUE (lista1.get_cantidad_campos() == 2);
	ASSERT_TRUE (archivo.agregar(&lista1) > 0);

	// El archivo ahora tiene 2 listas, una con 1 elem (23) y otra con 2 (23,11)

	//Agrego a las ref_listas el valor 1, que seria la posicion de la segunda lista en el archivo
	ref_listas[0]=1;
	//Reconstruimos las listas del archivo, o sea que vamos a agregarle a la lista 2 el id = 9
	ASSERT_TRUE(archivo.recontruir_listas(ref_listas, 1, 9) == RES_OK);
	ASSERT_TRUE(archivo.get_cantidad_listas() == 2);

	//Voy a ver que las listas tengan los id correctos

	//Obtengo la lista1 que se guardo en el archivo
	RegistroVariable lista3, lista4;
	archivo.devolver(&lista3, 0);
	ASSERT_TRUE(lista3.get_cantidad_campos() == 1);
	int idrecuperado = -1;
	lista3.recuperar_campo((char*)&idrecuperado,0);
	ASSERT_TRUE(idrecuperado == 23);
	//Obtengo la lista2 que se guardo en el archivo
	archivo.devolver(&lista4, 1);
	ASSERT_TRUE(lista4.get_cantidad_campos() == 3);
	idrecuperado = -1;
	lista4.recuperar_campo((char*)&idrecuperado,0);
	ASSERT_TRUE(idrecuperado == 23);
	lista4.recuperar_campo((char*)&idrecuperado,1);
	ASSERT_TRUE(idrecuperado == 11);
	lista4.recuperar_campo((char*)&idrecuperado,2);
	ASSERT_TRUE(idrecuperado == 9);
}

TEST_F(TestArchivoListas,Reconstruir_listas_por_frase)
{
	//TODO
}
