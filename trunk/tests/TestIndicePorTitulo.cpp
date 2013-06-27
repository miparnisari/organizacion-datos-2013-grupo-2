#include "../src/CapaLogica/Indices/IndiceInvertidoPorTitulo.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestIndicePorTitulo : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	std::string nombre_archivo;
    IndiceInvertidoPorTitulo indice_titulo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	nombre_archivo = "TestIndiceTitulo";
	ASSERT_TRUE(indice_titulo.crear_indice(nombre_archivo) == RES_OK);
	ASSERT_TRUE(indice_titulo.abrir_indice(nombre_archivo) == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(indice_titulo.cerrar_indice() == RES_OK);
	  ASSERT_TRUE(indice_titulo.eliminar_indice() == RES_OK);
  }

};

TEST_F(TestIndicePorTitulo,Agregar_Buscar)
{
    int id_doc_uno = 23;
    int id_doc_dos = 68;
    vector<int> id_docs_devueltos;

    // Titulo que no existe
    ASSERT_TRUE(indice_titulo.buscar("titulo inexistente",id_docs_devueltos)==RES_RECORD_DOESNT_EXIST);

    // Un titulo y su id_doc
    ASSERT_TRUE(indice_titulo.agregar("Dark side of the moon", id_doc_uno) == RES_OK);
    ASSERT_TRUE(indice_titulo.buscar("Dark side of the moon",id_docs_devueltos) ==RES_OK);
    ASSERT_TRUE(id_docs_devueltos.size() == 1);
    ASSERT_TRUE(id_docs_devueltos[0] == id_doc_uno);

    // El mismo titulo y otro id_doc
    ASSERT_TRUE(indice_titulo.agregar("Dark side of the moon", id_doc_dos) == RES_OK);
    ASSERT_TRUE(indice_titulo.buscar("Dark side of the moon",id_docs_devueltos) ==RES_OK);
    ASSERT_TRUE(id_docs_devueltos.size() == 2);
    ASSERT_TRUE(id_docs_devueltos[0] != id_docs_devueltos[1]);
    ASSERT_TRUE(id_docs_devueltos[0] == id_doc_uno);
    ASSERT_TRUE(id_docs_devueltos[1] == id_doc_dos);


    // Agregar un titulo con id_doc dos veces, deberia fallar
    // (para no tener valores duplicados)
    ASSERT_TRUE(indice_titulo.agregar("Dark side of the moon", id_doc_dos) == RES_RECORD_EXISTS);
}

TEST_F(TestIndicePorTitulo,Eliminar)
{
    int id_doc_uno = 23;
    vector<int> id_docs_devueltos;

    ASSERT_TRUE(indice_titulo.eliminar("Dark side of the moon",id_doc_uno) == RES_RECORD_DOESNT_EXIST);

    // Un titulo y su id_doc
    ASSERT_TRUE(indice_titulo.agregar("Dark side of the moon", id_doc_uno) == RES_OK);
    ASSERT_TRUE(indice_titulo.buscar("Dark side of the moon",id_docs_devueltos) ==RES_OK);
    ASSERT_TRUE(id_docs_devueltos.size() == 1);
    ASSERT_TRUE(id_docs_devueltos[0] == id_doc_uno);

    ASSERT_TRUE(indice_titulo.eliminar("Dark side of the moon",id_doc_uno) == RES_OK);

    ASSERT_TRUE(indice_titulo.buscar("Dark side of the moon",id_docs_devueltos) == RES_RECORD_DOESNT_EXIST);
    ASSERT_TRUE(id_docs_devueltos.size() == 0);
}
