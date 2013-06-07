#include "../src/CapaLogica/Indices/IndiceInvertidoPorAutor.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

const std::string ARCHIVO_ARBOL=	"IndicePorAutor.dat";
const std::string ARCHIVO_LISTAS=	"ListasPorAutor";

// To use a test fixture, derive a class from testing::Test.
class TestIndicePorAutor : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	ASSERT_TRUE(indice.crear_indice("") == RES_OK);
	ASSERT_TRUE(indice.abrir_indice("") == RES_OK);
	ASSERT_TRUE(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	ASSERT_TRUE(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(arbol.cerrar() == RES_OK);
	  ASSERT_TRUE(indice.borrar_indice() == RES_OK);
  }

  // A helper function
  // Crea un reg cancion con el autor que nos pasan por parametro
  void crear_reg_cancion(std::string autor, RegistroCancion &reg)
  {
  	std::string cancion = autor+"-2013-The wall-Ingles-Pink Floyd";
  	reg.cargar(cancion.c_str(), cancion.length());
  }

};

TEST_F(TestIndicePorAutor,Agregar_cancion)
{
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_autor;
    ClaveX clave, clave_aux;
    int id, ref_lista;
    indice.crear_indice("");

    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);

    //Veo si se guardo una lista con el IDcancion 23
    ASSERT_TRUE(listas.get_cantidad_listas() == 1);
    ASSERT_TRUE(listas.devolver(&lista,0) == RES_OK);
    ASSERT_TRUE(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&id,0);
    ASSERT_TRUE(id == 23);

    //Veo si se guardo el registro del autor en el arbol
    clave.set_clave("Pink Floyd");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    ASSERT_TRUE(arbol.buscar(reg_autor) == RES_OK);
    clave_aux.set_clave("Pink Floyd");
    ASSERT_TRUE(reg_autor.get_clave() == clave_aux);
    ASSERT_TRUE(reg_autor.get_cantidad_campos() == 2);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_autor.recuperar_campo(((char*)&ref_lista),0);
    ASSERT_TRUE(ref_lista == 0);
}

TEST_F(TestIndicePorAutor,Devolver_canciones_por_autor)
{
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_autor;
    ClaveX clave;
    int id;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 23);

	//Le pido al indice que me devuelva la lista de canciones de Pink Floyd
	ASSERT_TRUE(indice.buscar_autor("Pink Floyd", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
}

TEST_F(TestIndicePorAutor,Agregar_muchas_canciones)
{
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_autor;
    ClaveX clave;
    int ref_lista;

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("Queen", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 26);

    //Veo si se guardaron 2 listas, ya que solo tenemos 2 autores
    ASSERT_TRUE(listas.get_cantidad_listas() == 2);
    ASSERT_TRUE(listas.devolver(&lista1,0) == RES_OK);
    ASSERT_TRUE(listas.devolver(&lista2,0) == RES_OK);
    //La suma de las dos listas deberia dar la cantidad de registros almacenados
    ASSERT_TRUE((lista1.get_cantidad_campos()+lista2.get_cantidad_campos()) == 4);

	//Veo si se guardo el registro del autor Pink Floyd en el arbol
    clave.set_clave("Pink Floyd");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    ASSERT_TRUE(arbol.buscar(reg_autor) == RES_OK);
    clave.set_clave("Pink Floyd");
    ASSERT_TRUE(reg_autor.get_clave() == clave);
    ASSERT_TRUE(reg_autor.get_cantidad_campos() == 2);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
	reg_autor.recuperar_campo(((char*)&ref_lista),0);
	//Veo que guarde 3 canciones
	ASSERT_TRUE(listas.devolver(&lista1,ref_lista) == RES_OK);
	ASSERT_TRUE(lista1.get_cantidad_campos() == 3);

    //Veo si se guardo el registro del autor Queen en el arbol
    clave.set_clave("Queen");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    ASSERT_TRUE(arbol.buscar(reg_autor) == RES_OK);
    clave.set_clave("Queen");
    ASSERT_TRUE(reg_autor.get_clave() == clave);
    ASSERT_TRUE(reg_autor.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_autor.recuperar_campo(((char*)&ref_lista),0);
    //Veo que guarde 1 cancion
    ASSERT_TRUE(listas.devolver(&lista2,ref_lista) == RES_OK);
    ASSERT_TRUE(lista2.get_cantidad_campos() == 1);

}

TEST_F(TestIndicePorAutor,Devolver_muchas_canciones_por_autor)
{
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_autor;
    ClaveX clave;
    int id;

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("Queen", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 26);


    //Veo que me devuelvan las listas por cada autor con los IDcanciones correspondientes
    ASSERT_TRUE(indice.buscar_autor("Pink Floyd", lista1));
    ASSERT_TRUE(lista1.get_cantidad_campos() == 3);
    lista1.recuperar_campo((char*)&id, 0);
    ASSERT_TRUE(id == 23);
    lista1.recuperar_campo((char*)&id, 1);
    ASSERT_TRUE(id == 24);
    lista1.recuperar_campo((char*)&id, 2);
    ASSERT_TRUE(id == 26);

    ASSERT_TRUE(indice.buscar_autor("Queen", lista2));
    ASSERT_TRUE(lista2.get_cantidad_campos() == 3);
    lista2.recuperar_campo((char*)&id, 0);
    ASSERT_TRUE(id == 25);
}
