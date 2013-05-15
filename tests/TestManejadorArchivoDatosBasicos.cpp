/*
 * TestManejadorArchivoDatosBasicos.cpp
 *
 *  Created on: May 15, 2013
 *      Author: maine
 */

#include "TestManejadorArchivoDatosBasicos.h"

TestManejadorArchivoDatosBasicos::TestManejadorArchivoDatosBasicos()
{
}

TestManejadorArchivoDatosBasicos::~TestManejadorArchivoDatosBasicos()
{
}

void TestManejadorArchivoDatosBasicos::ejecutar()
{
	test_madb_leer();
}

void TestManejadorArchivoDatosBasicos::test_madb_leer()
{
	ManejadorArchivoDatosBasicos<int> madb_int;
	assert(madb_int.crear_archivo("madb_int.dat") == RES_OK);
	assert(madb_int.abrir_archivo("madb_int.dat","rb+") == RES_OK);

	int valor = 190;
	assert(madb_int.escribir(valor,0) == RES_OK);

	int valorLeido = -1;
	assert(madb_int.leer(&valorLeido,0) == RES_OK);
	assert(valorLeido == valor);

	assert(madb_int.cerrar_archivo() == RES_OK);

	print_test_ok("test_madb_leer");
}

