#include "../src/CapaLogica/HashingExtensible/Tabla.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
using namespace std;
using namespace utilitarios;

// To use a test fixture, derive a class from testing::Test.
class TestTabla : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	Tabla tabla;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	ASSERT_TRUE(tabla.crear("TablaDePrueba.dat") == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(tabla.eliminar() == RES_OK);
  }

  // A helper function that some test uses.

};


TEST_F(TestTabla,Crear)
{
	ManejadorArchivoDatosBasicos<int> archivo;

	ASSERT_TRUE(archivo.abrir_archivo("TablaDePrueba.dat", "rb") == RES_OK);
	ASSERT_TRUE(archivo.cerrar_archivo() == RES_OK);
}

TEST_F(TestTabla,Modificar_valor)
{
	ASSERT_TRUE(tabla.cambiar_valor(0,2) == RES_OK);
	ASSERT_TRUE(tabla.obtener_valor(0) == 2);
}

TEST_F(TestTabla,Dividir)
{
	ASSERT_TRUE(tabla.obtener_valor(0) == -1);
	ASSERT_TRUE(tabla.cambiar_valor(0,2) == RES_OK);
	ASSERT_TRUE(tabla.duplicar() == RES_OK);
	ASSERT_TRUE(tabla.dividir() == RES_OK);
}

TEST_F(TestTabla,Duplicar)
{
	ASSERT_TRUE(tabla.get_tamanio() == 1);
	ASSERT_TRUE(tabla.cambiar_valor(1,2) == RES_OK);
	ASSERT_TRUE(tabla.get_tamanio() == 2);
	ASSERT_TRUE(tabla.duplicar() == RES_OK);
	ASSERT_TRUE(tabla.get_tamanio() == 4);

	ASSERT_TRUE(tabla.obtener_valor(1) == 2);
}

TEST_F(TestTabla,Posiciones_correctas_al_duplicar)
{
	ASSERT_TRUE(tabla.duplicar() == RES_OK);
	ASSERT_TRUE(tabla.duplicar() == RES_OK);

	ASSERT_TRUE(tabla.cambiar_valor(0,6) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(1,9) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(2,65) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(3,34) == RES_OK);

	ASSERT_TRUE(tabla.duplicar() == RES_OK);

	ASSERT_TRUE(tabla.obtener_valor(4) == 6);
	ASSERT_TRUE(tabla.obtener_valor(5) == 9);
	ASSERT_TRUE(tabla.obtener_valor(6) == 65);
	ASSERT_TRUE(tabla.obtener_valor(7) == 34);
}

TEST_F(TestTabla,Posiciones_correctas_al_dividir)
{
	ASSERT_TRUE(tabla.duplicar() == RES_OK);
	ASSERT_TRUE(tabla.duplicar() == RES_OK);

	ASSERT_TRUE(tabla.cambiar_valor(0,4) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(1,2) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(2,97) == RES_OK);
	ASSERT_TRUE(tabla.cambiar_valor(3,22) == RES_OK);

	ASSERT_TRUE(tabla.dividir() == RES_OK);

	ASSERT_TRUE(tabla.obtener_valor(0) == 4);
	ASSERT_TRUE(tabla.obtener_valor(1) == 2);
}
