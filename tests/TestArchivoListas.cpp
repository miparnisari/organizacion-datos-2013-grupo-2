/*
 * TestArchivoListas.cpp
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */

#include "TestArchivoListas.h"
#define DIRECCION	"TestArchivoListas"
#define DIRECCION2 "TestArchivoListas.dat"

TestArchivoListas::TestArchivoListas()
{

}

TestArchivoListas::~TestArchivoListas()
{

}

void TestArchivoListas::ejecutar()
{
	test_archivo_listas_crear();
	test_archivo_listas_eliminar();
	test_archivo_listas_agregar();
	test_archivo_listas_devolver();
	test_archivo_listas_reconstruir_listas();
	test_archivo_listas_reconstruir_listas_por_frase();
}

void TestArchivoListas::test_archivo_listas_crear()
{
	ArchivoListas archivo;
	ManejadorRegistrosVariables manejador;
	//Creo el archivo
	assert(archivo.crear("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) == RES_OK);

	//Veosi se guardo algo en el archivo mediante el manejador
	assert(manejador.abrir_archivo(DIRECCION) == RES_OK);
	assert(manejador.get_cantidad_registros_ocupados() == 0);

	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_crear");
}

void TestArchivoListas::test_archivo_listas_eliminar()
{
	ArchivoListas archivo;
	//Creo el archivo
	assert(archivo.crear("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) == RES_OK);
	//Lo elimino
	assert(archivo.eliminar("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) != RES_OK);


	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_eliminar");
}

void TestArchivoListas::test_archivo_listas_agregar()
{
	ArchivoListas archivo;
	ManejadorRegistrosVariables manejador;
	RegistroVariable lista;
	int id = 23;
	//Creo el archivo
	assert(archivo.crear("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) == RES_OK);
	//Agrego un elemento a la lista
	lista.agregar_campo((char*)&id,sizeof(int));
	//Agrego la lista al archivo
	assert(archivo.agregar(&lista) != RES_ERROR);
	assert(archivo.get_cantidad_listas() == 1);

	//Veo que se guardo en el archivo mediante el manejador
	assert(manejador.abrir_archivo(DIRECCION2) == RES_OK);
	assert(manejador.get_cantidad_registros_ocupados() == 1);
	assert(manejador.get_registro_ocupado(&lista,0) > 0);
	assert(lista.get_cantidad_campos() == 1);
	int idrecuperado;
	lista.recuperar_campo((char*)&idrecuperado,0);
	assert(idrecuperado == 23);

	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_agregar");
}

void TestArchivoListas::test_archivo_listas_devolver()
{
	ArchivoListas archivo;
	ManejadorRegistrosVariables manejador;
	RegistroVariable lista;
	int id=23;
	//Creo el archivo
	assert(archivo.crear("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) == RES_OK);
	//Agrego un elemento a la lista
	lista.agregar_campo((char*)&id, sizeof(int));
	//Agrego la lista al archivo
	archivo.agregar(&lista);

	//Obtengo la lista que se guardo en el archivo
	assert(archivo.get_cantidad_listas() == 1);
	archivo.devolver(&lista, 0);
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id,0);
	assert(id == 23);

	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_devolver");
}

void TestArchivoListas::test_archivo_listas_reconstruir_listas()
{
	ArchivoListas archivo;
	ManejadorRegistrosVariables manejador;
	RegistroVariable lista1, lista2;
	int id=23;
	unsigned short* ref_listas = new unsigned short[1]();
	//Creo el archivo
	assert(archivo.crear("", DIRECCION) == RES_OK);
	//Lo intento abrir
	assert(archivo.abrir("", DIRECCION) == RES_OK);
	//Agrego 2 listas diferentes
	//Agrego un elemento a la lista1
	lista1.agregar_campo((char*)&id,sizeof(int));
	//Agrego la lista1 al archivo
	archivo.agregar(&lista1);
	//Agrego un elemento a la lista2
	id=11;
	lista1.agregar_campo((char*)&id,sizeof(int));
	//Agrego la lista2 al archivo
	archivo.agregar(&lista1);

	//Agrego a las ref_listas el valor 1, que seria la posicion de la segunda lista en el archivo
	ref_listas[0]=1;
	//Reconstruimos las listas del archivo, o sea que vamos a agregarle a la lista 2 el id = 9
	assert(archivo.recontruir_listas(ref_listas, 1, 9) == RES_OK);
	//Voy a ver que solo hayan 2 listas en el archivo
	assert(archivo.get_cantidad_listas() == 2);

	//Voy a ver que las listas tengan los id correctos

	//Obtengo la lista1 que se guardo en el archivo
	archivo.devolver(&lista1, 0);
	assert(lista1.get_cantidad_campos() == 1);
	lista1.recuperar_campo((char*)&id,0);
	assert(id == 23);
	//Obtengo la lista2 que se guardo en el archivo
	archivo.devolver(&lista2, 1);
	assert(lista2.get_cantidad_campos() == 2);
	lista2.recuperar_campo((char*)&id,0);
	assert(id == 11);
	lista2.recuperar_campo((char*)&id,1);
	assert(id == 9);


	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_reconstruir_listas");
}

void TestArchivoListas::test_archivo_listas_reconstruir_listas_por_frase()
{

	this->eliminar_archivos();
	print_test_ok("test_archivo_listas_reconstruir_listas_por_frase");
}

void TestArchivoListas::eliminar_archivos()
{
	//Elimina los archivos que se usan en cada archivo
	ManejadorArchivos manejador;
	manejador.eliminar_archivo(DIRECCION2);
}

