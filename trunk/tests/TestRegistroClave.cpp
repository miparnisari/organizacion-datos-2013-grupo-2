/*
 * TestRegistroClave.cpp
 *
 *  Created on: May 12, 2013
 *      Author: maine
 */

#include "TestRegistroClave.h"

TestRegistroClave::TestRegistroClave() {

}

TestRegistroClave::~TestRegistroClave() {
}

void TestRegistroClave::ejecutar()
{
	test_registro_clave_guardar_y_leer();
}

void TestRegistroClave::test_registro_clave_guardar_y_leer()
{
	RegistroClave registro;
	ClaveX claveescrita;
	claveescrita.set_clave("una artista");
	std::string campo = "madonna";

	registro.set_clave(claveescrita);
	registro.agregar_campo(campo.c_str(), campo.size());

	assert(registro.get_cantidad_campos() == 1);
	assert(registro.get_clave() == claveescrita);

	std::cout << "registro.get_tamanio_campo(0) = " << registro.get_tamanio_campo(0) << std::endl;
	assert(registro.get_tamanio_campo(0) != RES_ERROR);
	assert((unsigned)registro.get_tamanio_campo(0) == campo.size());

	for (unsigned int i = 0; i < registro.get_cantidad_campos(); i++)
	{
		std::cout << "tamanio campo = " << registro.get_tamanio_campo(i) << std::endl;
		char* buffer = new char[registro.get_tamanio_campo(i)]();

		registro.recuperar_campo(buffer,i);

		assert(strcmp(buffer,"madonna") == 0);

		delete[] buffer;
	}

	print_test_ok("test_registro_clave_guardar_y_leer");
}
