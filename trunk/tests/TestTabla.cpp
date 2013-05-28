/*
 * TestTabla.cpp
 *
 *  Created on: May 12, 2013
 *      Author: nico
 */

#include "TestTabla.h"
#define DIRECCION "TablaDePrueba.dat"
#define DIRECCION2 "TablaDePrueba"
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
	test_tabla_hash_posiciones_correctas_al_duplicar();
	test_tabla_hash_posiciones_correctas_al_dividir();
	test_tabla_hash_eliminar_tabla();
}

void TestTabla::test_tabla_hash_crear(){
	Tabla unaTabla;
	assert(unaTabla.crear(DIRECCION) == RES_OK);
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

void TestTabla::test_tabla_hash_eliminar_tabla(){
	Tabla unaTabla;

	assert(unaTabla.crear(DIRECCION) == RES_OK);
	assert(unaTabla.crear(DIRECCION) == RES_OK);

	assert(unaTabla.eliminar() == RES_OK);
	print_test_ok("test_tabla_hash_eliminar_tabla");
}


void TestTabla::test_tabla_hash_posiciones_correctas_al_duplicar(){
	Tabla unaTabla;

	assert(unaTabla.crear(DIRECCION) == RES_OK);

	assert(unaTabla.duplicar() == RES_OK);
	assert(unaTabla.duplicar() == RES_OK);

	assert(unaTabla.cambiar_valor(0,6) == RES_OK);
	assert(unaTabla.cambiar_valor(1,9) == RES_OK);
	assert(unaTabla.cambiar_valor(2,65) == RES_OK);
	assert(unaTabla.cambiar_valor(3,34) == RES_OK);

	assert(unaTabla.duplicar() == RES_OK);

	assert(unaTabla.obtener_valor(4) == 6);
	assert(unaTabla.obtener_valor(5) == 9);
	assert(unaTabla.obtener_valor(6) == 65);
	assert(unaTabla.obtener_valor(7) == 34);

	print_test_ok("test_tabla_hash_posiciones_correctas_al_duplicar");
}

/*Cabe destacar que aca se prueba que la tabla divida los valores y salve los
 *  adecuados, osea, los de la primera mitad, pero el hash se encarga de
 *  chequear las condiciones de si se puede divirdir sin perder informacion
 *  o no*/
void TestTabla::test_tabla_hash_posiciones_correctas_al_dividir(){
	Tabla unaTabla;

	assert(unaTabla.crear(DIRECCION) == RES_OK);

	assert(unaTabla.duplicar() == RES_OK);
	assert(unaTabla.duplicar() == RES_OK);

	assert(unaTabla.cambiar_valor(0,4) == RES_OK);
	assert(unaTabla.cambiar_valor(1,2) == RES_OK);
	assert(unaTabla.cambiar_valor(2,97) == RES_OK);
	assert(unaTabla.cambiar_valor(3,22) == RES_OK);

	assert(unaTabla.dividir() == RES_OK);

	assert(unaTabla.obtener_valor(0) == 4);
	assert(unaTabla.obtener_valor(1) == 2);

	print_test_ok("test_tabla_hash_posiciones_correctas_al_dividir");
}

