#include "../src/CapaFisica/ManejadorArchivoDatosBasicos.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestManejadorArchivoDatosBasicos : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ManejadorArchivoDatosBasicos<int> madb_int;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  ASSERT_TRUE(madb_int.crear_archivo("madb_int.dat") == RES_OK);
	  ASSERT_TRUE(madb_int.abrir_archivo("madb_int.dat","rb+") == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(madb_int.cerrar_archivo() == RES_OK);
	  ASSERT_TRUE(madb_int.eliminar_archivo("madb_int.dat") == RES_OK);
  }

};

TEST_F(TestManejadorArchivoDatosBasicos,Leer_escribir)
{
	// Arrange
	int valorUno = 190;
	int valorDos = -800;
	int valorTres = 65;

	// Act
	ASSERT_TRUE(madb_int.escribir(valorUno,0) == RES_OK);
	ASSERT_TRUE(madb_int.escribir(valorDos,1) == RES_OK);
	ASSERT_TRUE(madb_int.agregar(valorTres) == 2);

	// Assert
	int valorLeidoUno = -1;
	int valorLeidoDos = -1;
	int valorLeidoTres = -1;
	ASSERT_TRUE(madb_int.leer(&valorLeidoUno,0) == RES_OK);
	ASSERT_TRUE(madb_int.leer(&valorLeidoDos,1) == RES_OK);
	ASSERT_TRUE(madb_int.leer(&valorLeidoTres,2) == RES_OK);

	ASSERT_TRUE(valorLeidoUno == valorUno);
	ASSERT_TRUE(valorLeidoDos == valorDos);
	ASSERT_TRUE(valorLeidoTres == valorTres);

}

TEST_F(TestManejadorArchivoDatosBasicos,Truncar)
{
	// Arrange
	int valorUno = 190;
	int valorDos = -800;

	// Act
	ASSERT_TRUE(madb_int.escribir(valorUno,0) == RES_OK);
	ASSERT_TRUE(madb_int.escribir(valorDos,1) == RES_OK);
	ASSERT_TRUE(madb_int.get_tamanio_archivo() == 2*sizeof(int));

	// ASSERT_TRUE
	ASSERT_TRUE(madb_int.truncar(1) == RES_OK);
	ASSERT_TRUE(madb_int.get_tamanio_archivo() == 1*sizeof(int));
	ASSERT_TRUE(madb_int.leer(NULL,2) == RES_ERROR);

}
