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
//	test_crear_tabla();
//	test_modificar_valor_de_tabla();
//	test_dividir();
	test_duplicacion();
}

void TestTabla::test_crear_tabla(){
	Tabla unaTabla(DIRECCION);
	ManejadorArchivoDatosBasicos<int> archivo;
	int seAbrio = 0;

	unaTabla.crear_archivo_tabla(DIRECCION);
	seAbrio = archivo.abrir_archivo(DIRECCION, "rb");

	assert(seAbrio != RES_ERROR);
    print_test_ok("crear_tabla");
    unaTabla.eliminar_tabla();

    print_test_ok("test_crear_tabla");
}

void TestTabla::test_modificar_valor_de_tabla(){
	Tabla unaTabla(DIRECCION);

	assert(unaTabla.cambiar_valor(1,2) == RES_OK);

	std::cout << "obtener valor" << unaTabla.obtener_valor(1) << std::endl;
	assert(unaTabla.obtener_valor(1) == 2);
	print_test_ok("modificar_valor");
	unaTabla.eliminar_tabla();

	print_test_ok("test_modificar_valor_de_tabla");
}

void TestTabla::test_duplicacion(){
	Tabla unaTabla(DIRECCION);

	assert(unaTabla.cambiar_valor(1,2) == RES_OK);
	assert( unaTabla.duplicar_tabla() == RES_OK);

	assert(unaTabla.obtener_valor(2) == 2);
	print_test_ok("duplicar_tabla");
	unaTabla.eliminar_tabla();

	print_test_ok("test_duplicacion");
}

void TestTabla::test_dividir(){
	Tabla unaTabla(DIRECCION);

	assert(unaTabla.cambiar_valor(1,2) == RES_OK);
	unaTabla.duplicar_tabla();
	unaTabla.duplicar_tabla();

	unaTabla.dividir_tabla();

	assert(unaTabla.get_tamanio() == 2);
	print_test_ok("dividir_tabla");
	unaTabla.eliminar_tabla();

	print_test_ok("test_dividir");
}
