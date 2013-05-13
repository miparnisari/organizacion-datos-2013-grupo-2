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

	assert(registro.get_cantidad_campos() == 2);
	assert(registro.get_clave() == claveescrita);
	assert(registro.get_tamanio_campo(1) != RES_ERROR);
	assert((unsigned)registro.get_tamanio_campo(1) == campo.size());

	for (unsigned int i = 1; i < registro.get_cantidad_campos(); i++)
	{
		char* buffer = new char[registro.get_tamanio_campo(i)+1]();
		buffer[registro.get_tamanio_campo(i)] = '\0';
		registro.recuperar_campo(buffer,i);

		if (i == 0)
			assert(strcmp(buffer,"una artista") == 0);
		else
			assert(strcmp(buffer,"madonna") == 0);

		delete[] buffer;
	}

	print_test_ok("test_registro_clave_guardar_y_leer");
}
