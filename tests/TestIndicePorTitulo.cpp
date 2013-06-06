#include "../src/CapaLogica/Indices/IndiceInvertidoPorTitulo.h"
#include "../src/CapaLogica/Indices/ArchivoListas.h"
#include "../src/CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

const std::string ARCHIVO_HASH = "IndicePorTitulo.dat";
const std::string ARCHIVO_LISTAS = "ListasPorTitulo";

// To use a test fixture, derive a class from testing::Test.
class TestIndicePorTitulo : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
    IndiceInvertidoPorTitulo indice;
    ArchivoListas listas;
    HashingExtensible hash;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	ASSERT_TRUE(indice.crear_indice("") == RES_OK);
	ASSERT_TRUE(indice.abrir_indice("") == RES_OK);
    ASSERT_TRUE(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    ASSERT_TRUE(hash.abrir_archivo(ARCHIVO_HASH) == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(indice.borrar_indice() == RES_OK);
  }

  // A helper function
  void crear_reg_cancion(std::string titulo, RegistroCancion &reg)
  {
      //Crea un reg cancion con el autor que nos pasan por parametro
	reg.limpiar_campos();
	reg.limpiar_buffer();
  	std::string cancion = "Pink Floyd-2013-"+titulo+"-en-Pink Floyd";
  	reg.cargar(cancion.c_str(), cancion.length());
  }

};

TEST_F(TestIndicePorTitulo,Agregar_cancion)
{
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_titulo;
    ClaveX clave, clave_aux;
    int id, ref_lista;

    this->crear_reg_cancion("Pink Floyd", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 23) == RES_OK);

    //Veo si se guardo una lista con el IDcancion 23
    ASSERT_TRUE(listas.get_cantidad_listas() == 1);
    ASSERT_TRUE(listas.devolver(&lista,0) == RES_OK);
    ASSERT_TRUE(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&id,0);
    ASSERT_TRUE(id == 23);

    //Veo si se guardo el registro del autor en el arbol
    clave.set_clave("The final cut");
    //Busco el registro en el arbol
    ASSERT_TRUE(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave_aux.set_clave("The final cut");
    ASSERT_TRUE(reg_titulo.get_clave() == clave_aux);
    ASSERT_TRUE(reg_titulo.get_cantidad_campos() == 2);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_titulo.recuperar_campo(((char*)&ref_lista),0);
    ASSERT_TRUE(ref_lista == 0);

}

TEST_F(TestIndicePorTitulo,Devolver_canciones_por_titulo)
{
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_titulo;
    ClaveX clave;
    int id;

    this->crear_reg_cancion("The final cut", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 23) == RES_OK);
	//Veo si se crearon los archivos del indice
	ASSERT_TRUE(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	ASSERT_TRUE(hash.abrir_archivo(ARCHIVO_HASH)  == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones con titulo The final cut
	ASSERT_TRUE(indice.buscar_titulo("The final cut", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
}

TEST_F(TestIndicePorTitulo,Agregar_muchas_canciones)
{
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_titulo;
    ClaveX clave;
    int ref_lista;

    //Agrego tres canciones con el mismo titulo y uno distinto
    this->crear_reg_cancion("The final cut", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 23) == RES_OK);
    this->crear_reg_cancion("The final cut", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 24) == RES_OK);
    this->crear_reg_cancion("The Trial", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 25) == RES_OK);
    this->crear_reg_cancion("The final cut", cancion);
    ASSERT_TRUE(indice.agregar_cancion(cancion, 26) == RES_OK);

    //Veo si se guardaron 2 listas, ya que solo tenemos 2 titulos
    ASSERT_TRUE(listas.get_cantidad_listas() == 2);
    ASSERT_TRUE(listas.devolver(&lista1,0) == RES_OK);
    ASSERT_TRUE(listas.devolver(&lista2,0) == RES_OK);
    //La suma de las dos listas deberia dar la cantidad de canciones almacenados
    ASSERT_TRUE((lista1.get_cantidad_campos()+lista2.get_cantidad_campos()) == 4);

	//Veo si se guardo el registro de titulo The final cut en el hash
    clave.set_clave("The final cut");
    //Busco el registro en el hash
    ASSERT_TRUE(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave.set_clave("The final cut");
    ASSERT_TRUE(reg_titulo.get_clave() == clave);
    ASSERT_TRUE(reg_titulo.get_cantidad_campos() == 2);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
	reg_titulo.recuperar_campo(((char*)&ref_lista),0);
	//Veo que guarde 3 canciones
	ASSERT_TRUE(listas.devolver(&lista1,ref_lista) == RES_OK);
	ASSERT_TRUE(lista1.get_cantidad_campos() == 3);

    //Veo si se guardo el registro del titulo The trial en el hash
    clave.set_clave("The Trial");
    //Busco el registro en el hash
    ASSERT_TRUE(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave.set_clave("The Trial");
    ASSERT_TRUE(reg_titulo.get_clave() == clave);
    ASSERT_TRUE(reg_titulo.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_titulo.recuperar_campo(((char*)&ref_lista),0);
    //Veo que guarde 1 cancion
    ASSERT_TRUE(listas.devolver(&lista2,ref_lista) == RES_OK);
    ASSERT_TRUE(lista2.get_cantidad_campos() == 1);

}

TEST_F(TestIndicePorTitulo,Devolver_muchas_canciones_por_titulo)
{
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_titulo;
    ClaveX clave;
    int id;

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("The final cut", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("The Trial", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 26);

    //Veo que me devuelvan las listas por cada autor con los IDcanciones correspondientes
    ASSERT_TRUE(indice.buscar_titulo("The final cut", lista1));
    ASSERT_TRUE(lista1.get_cantidad_campos() == 3);
    lista1.recuperar_campo((char*)&id, 0);
    ASSERT_TRUE(id == 23);
    lista1.recuperar_campo((char*)&id, 1);
    ASSERT_TRUE(id == 24);
    lista1.recuperar_campo((char*)&id, 2);
    ASSERT_TRUE(id == 26);

    ASSERT_TRUE(indice.buscar_titulo("The Trial", lista2));
    ASSERT_TRUE(lista2.get_cantidad_campos() == 3);
    lista2.recuperar_campo((char*)&id, 0);
    ASSERT_TRUE(id == 25);

}
