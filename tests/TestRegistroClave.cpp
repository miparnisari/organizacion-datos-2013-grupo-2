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
	test_registro_clave_empaquetar_desempaquetar();
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

void TestRegistroClave::test_registro_clave_empaquetar_desempaquetar()
{
	ClaveX clave;
	clave.set_clave("una clave 123");
	std::string campo = "un campo 456";

	RegistroClave registro;
	registro.set_clave(clave);
	registro.agregar_campo(campo.c_str(), campo.size());

	char* buffer = new char[registro.get_tamanio_empaquetado() +1]();
	buffer[registro.get_tamanio_empaquetado()] = '\0';

	assert(registro.empaquetar(buffer) == RES_OK);

	RegistroClave registroDesempaquetado;
	registroDesempaquetado.desempaquetar(buffer);

	// Recupero la clave
	assert (registroDesempaquetado.get_clave() == clave);

	// Recupero el campo
	char* campoRecuperado = new char[registroDesempaquetado.get_tamanio_campo(0) +1]();
	campoRecuperado[registroDesempaquetado.get_tamanio_campo(0)] = '\0';

	registroDesempaquetado.recuperar_campo(campoRecuperado,1);

	assert( strcmp(campoRecuperado,campo.c_str()) == 0);

	delete[] buffer;
	delete[] campoRecuperado;

	print_test_ok("test_registro_clave_empaquetar_desempaquetar");

}
