/*
 * IndiceInvertidoPorTitulo.cpp
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */

#include "TestIndicePorTitulo.h"
#define ARCHIVO_HASH	"IndicePorTitulo"
#define ARCHIVO_LISTAS	"ListasPorTitulo.dat"

TestIndicePorTitulo::TestIndicePorTitulo()
{

}

TestIndicePorTitulo::~TestIndicePorTitulo()
{

}

void TestIndicePorTitulo::ejecutar()
{
    test_indice_titulo_crear_indice();
    test_indice_titulo_agregar_cancion();
    test_indice_titulo_devolver_canciones_por_autor();
    test_indice_titulo_borrar_indice();
    test_indice_titulo_agregar_muchas_canciones();
    test_indice_titulo_devolver_muchas_canciones_por_autor();
}

void TestIndicePorTitulo::test_indice_titulo_crear_indice()
{
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    HashingExtensible hash;
    indice.crear_indice("");
    //Vemos si se creo el archivo de listas
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    //Vemos si se creo el hash
    assert(hash.abrir_archivo(ARCHIVO_HASH) == RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_titulo_crear_indice");
}

void TestIndicePorTitulo::test_indice_titulo_agregar_cancion()
{
    IndiceInvertidoPorTitulo indice;
    ArchivoListas listas;
    HashingExtensible hash;
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_titulo;
    ClaveX clave, clave_aux;
    int id, ref_lista;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(hash.abrir_archivo(ARCHIVO_HASH) == RES_OK);
    //Veo si se guardo una lista con el IDcancion 23
    assert(listas.get_cantidad_listas() == 1);
    assert(listas.devolver(&lista,0) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&id,0);
    assert(id == 23);

    //Veo si se guardo el registro del autor en el arbol
    clave.set_clave("The final cut");
    //Busco el registro en el arbol
    assert(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave_aux.set_clave("The final cut");
    assert(reg_titulo.get_clave() == clave_aux);
    assert(reg_titulo.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_titulo.recuperar_campo(((char*)&ref_lista),0);
    assert(ref_lista == 0);

    this->eliminar_archivos();
    print_test_ok("test_indice_titulo_agregar_cancion");
}

void TestIndicePorTitulo::test_indice_titulo_devolver_canciones_por_autor()
{
	IndiceInvertidoPorTitulo indice;
	HashingExtensible hash;
    ArchivoListas listas;
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_titulo;
    ClaveX clave;
    int id;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("The final cut", cancion);
	indice.agregar_cancion(cancion, 23);
	//Veo si se crearon los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	assert(hash.abrir_archivo(ARCHIVO_HASH)  == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones con titulo The final cut
	assert(indice.buscar_titulo("The final cut", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
    this->eliminar_archivos();
    print_test_ok("test_devolver_canciones_por_autor");
}

void TestIndicePorTitulo::test_indice_titulo_borrar_indice()
{
	IndiceInvertidoPorTitulo indice;
	HashingExtensible hash;
	ArchivoListas listas;
	indice.crear_indice("");
	//Veo si se crearon los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	assert(hash.abrir_archivo(ARCHIVO_HASH)  == RES_OK);
	//Elimino el indice
	indice.borrar_indice();
	//Veo si se eliminaron los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) != RES_OK);
	assert(hash.abrir_archivo(ARCHIVO_HASH)  != RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_titulo_borrar_indice");
}

void TestIndicePorTitulo::test_indice_titulo_agregar_muchas_canciones()
{
	IndiceInvertidoPorTitulo indice;
	HashingExtensible hash;
    ArchivoListas listas;
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_titulo;
    ClaveX clave;
    int ref_lista;
    indice.crear_indice("");
    indice.abrir_indice("");

    //Agrego tres canciones con el mismo titulo y uno distinto
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("The final cut", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("The Trial", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 26);

   	//Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(hash.abrir_archivo(ARCHIVO_HASH)  == RES_OK);
    //Veo si se guardaron 2 listas, ya que solo tenemos 2 titulos
    assert(listas.get_cantidad_listas() == 2);
    assert(listas.devolver(&lista1,0) == RES_OK);
    assert(listas.devolver(&lista2,0) == RES_OK);
    //La suma de las dos listas deberia dar la cantidad de canciones almacenados
    assert((lista1.get_cantidad_campos()+lista2.get_cantidad_campos()) == 4);

	//Veo si se guardo el registro de titulo The final cut en el hash
    clave.set_clave("The final cut");
    //Busco el registro en el hash
    assert(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave.set_clave("The final cut");
    assert(reg_titulo.get_clave() == clave);
    assert(reg_titulo.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
	reg_titulo.recuperar_campo(((char*)&ref_lista),0);
	//Veo que guarde 3 canciones
	assert(listas.devolver(&lista1,ref_lista) == RES_OK);
	assert(lista1.get_cantidad_campos() == 3);

    //Veo si se guardo el registro del titulo The trial en el hash
    clave.set_clave("The Trial");
    //Busco el registro en el hash
    assert(hash.devolver(clave, &reg_titulo) == RES_OK);
    clave.set_clave("The Trial");
    assert(reg_titulo.get_clave() == clave);
    assert(reg_titulo.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_titulo.recuperar_campo(((char*)&ref_lista),0);
    //Veo que guarde 1 cancion
    assert(listas.devolver(&lista2,ref_lista) == RES_OK);
    assert(lista2.get_cantidad_campos() == 1);

    this->eliminar_archivos();
    print_test_ok("test_indice_titulo_agregar_muchas_canciones");
}

void TestIndicePorTitulo::test_indice_titulo_devolver_muchas_canciones_por_autor()
{
	IndiceInvertidoPorTitulo indice;
	HashingExtensible hash;
    ArchivoListas listas;
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_titulo;
    ClaveX clave;
    int ref_lista, id;
    indice.crear_indice("");
    indice.abrir_indice("");

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("The final cut", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("The Trial", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("The final cut", cancion);
    indice.agregar_cancion(cancion, 26);

    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(hash.abrir_archivo(ARCHIVO_HASH) == RES_OK);

    //Veo que me devuelvan las listas por cada autor con los IDcanciones correspondientes
    assert(indice.buscar_titulo("The final cut", lista1));
    assert(lista1.get_cantidad_campos() == 3);
    lista1.recuperar_campo((char*)&id, 0);
    assert(id == 23);
    lista1.recuperar_campo((char*)&id, 1);
    assert(id == 24);
    lista1.recuperar_campo((char*)&id, 2);
    assert(id == 26);

    assert(indice.buscar_titulo("The Trial", lista2));
    assert(lista2.get_cantidad_campos() == 3);
    lista2.recuperar_campo((char*)&id, 0);
    assert(id == 25);

    this->eliminar_archivos();
    print_test_ok("test_indice_titulo_devolver_muchas_canciones_por_autor");
}

void TestIndicePorTitulo::eliminar_archivos()
{
    //Elimina los archivos que se usan en cada archivo
    ManejadorArchivos manejador;
    HashingExtensible hash;
    manejador.eliminar_archivo(ARCHIVO_LISTAS);
    hash.eliminar_archivo();
}

void TestIndicePorTitulo::crear_reg_cancion(const char *titulo, RegistroCancion &reg)
{
    //Crea un reg cancion con el autor que nos pasan por parametro
	std::string cancion = "Pink Floyd-2013-"+titulo+"-Ingles-Pink Floyd";
    reg.cargar(cancion.c_str(), cancion.length());
}


