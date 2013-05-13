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
	ClaveX claveescrita, claveleida;
	claveescrita.set_clave("mi clave952");


	registro.set_clave(claveescrita);
	registro.get_clave(claveleida);
	assert(claveescrita == claveleida);

	std::string string;
	claveleida.get_clave(string);
	assert(string == "mi clave952");

	std::string campo = "123mi campo U2";
	registro.agregar_campo(campo.c_str(), campo.size());

	for (unsigned int i = 0; i < registro.get_cantidad_campos(); i++)
	{
		char* buffer = new char[registro.get_tamanio_campo(i) +1]();
		buffer[registro.get_tamanio_campo(i)] = '\0';
		registro.recuperar_campo(buffer,i);
		if (i == 0)
		{
			ClaveX unaClave;
			unaClave.desempaquetar(buffer,registro.get_tamanio_campo(0));
		}
		else {
			delete[] buffer;
		}
	}

	print_test_ok("test_registro_clave_guardar_y_leer");
}
