/*
 * TestRegistroVariable.cpp
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#include "TestRegistroVariable.h"

TestRegistroVariable::TestRegistroVariable()
	: Test()
{
}

TestRegistroVariable::~TestRegistroVariable()
{
}

void TestRegistroVariable::ejecutar()
{
	test_agregar_campos();
	test_eliminar();
	test_empaquetar_desempaquetar();
	test_recuperar_campos();
}

void TestRegistroVariable::test_agregar_campos()
{
	std::string campos[3]= {"martin", "mateo", "hector"};
	int tamanioCampos = 6+5+6;
	int cantidadCampos = 3;

	RegistroVariable rv;

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.get_cantidad_campos() == cantidadCampos);
	assert(rv.get_tamanio_campo(0) == 6);
	assert(rv.get_tamanio_campo(1) == 5);
	assert(rv.get_tamanio_campo(2) == 6);
	assert(rv.get_tamanio() == tamanioCampos + cantidadCampos * sizeof(unsigned short));

	{
		RegistroVariable rv1;

		const unsigned short CANTIDAD_CAMPOS= 3;
		string campoAcumulado= "d";
		for(unsigned short i=0;i<CANTIDAD_CAMPOS;i++){
			campoAcumulado+= "aa";
			rv1.agregar_campo(campoAcumulado.c_str() , campoAcumulado.length());
		}

		RegistroVariable rv2(rv1);
		assert( rv1.get_cantidad_campos()== rv2.get_cantidad_campos() &&
				rv1.get_tamanio()== rv2.get_tamanio() );
		for(unsigned short i=0;i<CANTIDAD_CAMPOS;i++){
			assert(rv1.get_tamanio_campo(i)== rv2.get_tamanio_campo(i));
		}


	}


	print_test_ok("test_agregar_campos_registro_variable");
}

void TestRegistroVariable::test_eliminar()
{
	std::string unicoCampo = "unico campo";
	int tamanioCampo = unicoCampo.size();

	RegistroVariable rv;
	rv.agregar_campo(unicoCampo.c_str(),strlen(unicoCampo.c_str()));
	assert(rv.get_cantidad_campos() == 1);
	assert(rv.get_tamanio() == tamanioCampo + sizeof(unsigned short));
	assert(rv.get_tamanio_campo(0) == tamanioCampo);

	unsigned short tamanioAntesDeBorrar = rv.get_tamanio();

	assert(rv.eliminar() == RES_OK);
	assert(rv.get_cantidad_campos() == 0);
	assert(rv.get_tamanio() == tamanioAntesDeBorrar);
	assert(rv.get_tamanio_campo(0) == RES_ERROR);

	print_test_ok("test_eliminar_registro_variable");
}

void TestRegistroVariable::test_empaquetar_desempaquetar()
{
	std::string campos[]= {"martin", "mateo", "hector"};
	unsigned short int cantidadCampos = 3;
	std::string mostrar = "martin|mateo|hector|";

	RegistroVariable rv;

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.mostrar() == mostrar);

	char buffer[64];
	rv.empaquetar(buffer);
	RegistroVariable rv2;

	assert(rv2.desempaquetar(buffer) == RES_OK);
	assert(rv2.get_tamanio() == rv.get_tamanio() );
	assert(rv2.mostrar() == mostrar);
	assert(rv.fue_eliminado()== false );

	print_test_ok("test_empaquetar_desempaquetar_registro_variable");
}


void TestRegistroVariable::test_recuperar_campos()
{
	std::string campos[]= {"martin", "mateo", "hector"};

	RegistroVariable rv3;
	rv3.agregar_campo(campos[0].c_str(),strlen(campos[0].c_str()));
	rv3.agregar_campo(campos[1].c_str(),strlen(campos[1].c_str()));
	rv3.agregar_campo(campos[2].c_str(),strlen(campos[2].c_str()));

	char* campoRecuperado;
	for (unsigned short int i = 0; i < rv3.get_cantidad_campos(); i ++)
	{
		campoRecuperado = new char[rv3.get_tamanio_campo(i) +1]();
		rv3.recuperar_campo(campoRecuperado,i);
		const char* copia = campoRecuperado;
		const char* campo = campos[i].c_str();
		assert (strcmp(copia,campo) == 0);
		delete[] campoRecuperado;
	}

	print_test_ok("test_recuperar_campos_registro_variable");
}
