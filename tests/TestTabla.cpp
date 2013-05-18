/*
 * TestTabla.cpp
 *
 *  Created on: May 12, 2013
 *      Author: nico
 */

#include "TestTabla.h"
#define DIRECCION "TablaDePrueba.dat"
using namespace std;
using namespace utilitarios;

TestTabla::TestTabla() {

}

TestTabla::~TestTabla() {
}

void TestTabla::ejecutar(){
	test_tabla_hash_crear();
	test_tabla_hash_modificar_valor();
	test_tabla_hash_dividir();
	test_tabla_hash_duplicacion();
}

void TestTabla::test_tabla_hash_crear(){
	Tabla unaTabla;
	ManejadorArchivoDatosBasicos<int> archivo;


	assert(unaTabla.crear(DIRECCION) == RES_OK);
	assert(archivo.abrir_archivo(DIRECCION, "rb") == RES_OK);
	assert(archivo.cerrar_archivo() == RES_OK);
	assert(unaTabla.eliminar() == RES_OK);

    print_test_ok("test_tabla_hash_crear");
}

void TestTabla::test_tabla_hash_modificar_valor(){
	Tabla unaTabla;
	assert(unaTabla.crear(DIRECCION) == RES_OK);
	assert(unaTabla.cambiar_valor(0,2) == RES_OK);
	assert(unaTabla.obtener_valor(0) == 2);
	assert(unaTabla.eliminar() == RES_OK);

	print_test_ok("test_tabla_hash_modificar_valor");
}

void TestTabla::test_tabla_hash_dividir(){
	Tabla unaTabla;
	assert(unaTabla.crear(DIRECCION) == RES_OK);

	assert(unaTabla.obtener_valor(0) == -1);
	assert(unaTabla.cambiar_valor(0,2) == RES_OK);
	assert(unaTabla.duplicar() == RES_OK);
	assert(unaTabla.dividir() == RES_OK);
	assert(unaTabla.eliminar() == RES_OK);

	print_test_ok("test_tabla_hash_dividir");
}

void TestTabla::test_tabla_hash_duplicacion(){
	Tabla unaTabla;
	assert(unaTabla.crear(DIRECCION) == RES_OK);

	assert(unaTabla.get_tamanio() == 1);
	assert(unaTabla.cambiar_valor(1,2) == RES_OK);
	assert(unaTabla.get_tamanio() == 2);
	assert(unaTabla.duplicar() == RES_OK);
	assert(unaTabla.get_tamanio() == 4);

	assert(unaTabla.obtener_valor(1) == 2);
	assert(unaTabla.eliminar() == RES_OK);

	print_test_ok("test_tabla_hash_duplicacion");
}
