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
	test_madb_leer_escribir();
	test_madb_truncar();
}

void TestManejadorArchivoDatosBasicos::test_madb_leer_escribir()
{
	// Arrange
	ManejadorArchivoDatosBasicos<int> madb_int;
	assert(madb_int.crear_archivo("madb_int.dat") == RES_OK);
	assert(madb_int.abrir_archivo("madb_int.dat","rb+") == RES_OK);

	int valorUno = 190;
	int valorDos = -800;
	int valorTres = 65;

	// Act
	assert(madb_int.escribir(valorUno,0) == RES_OK);
	assert(madb_int.escribir(valorDos,1) == RES_OK);
	assert(madb_int.agregar(valorTres) == 2);
	assert(madb_int.cerrar_archivo() == RES_OK);

	// Assert
	assert(madb_int.abrir_archivo("madb_int.dat","rb+") == RES_OK);
	int valorLeidoUno = -1;
	int valorLeidoDos = -1;
	int valorLeidoTres = -1;
	assert(madb_int.leer(&valorLeidoUno,0) == RES_OK);
	assert(madb_int.leer(&valorLeidoDos,1) == RES_OK);
	assert(madb_int.leer(&valorLeidoTres,2) == RES_OK);

	assert(valorLeidoUno == valorUno);
	assert(valorLeidoDos == valorDos);
	assert(valorLeidoTres == valorTres);

	assert(madb_int.cerrar_archivo() == RES_OK);

	print_test_ok("test_madb_leer");
}

void TestManejadorArchivoDatosBasicos::test_madb_truncar()
{
	// Arrange
	ManejadorArchivoDatosBasicos<int> madb_int;
	assert(madb_int.crear_archivo("madb_int_trunc.dat") == RES_OK);
	assert(madb_int.abrir_archivo("madb_int_trunc.dat","rb+") == RES_OK);

	int valorUno = 190;
	int valorDos = -800;

	// Act
	assert(madb_int.escribir(valorUno,0) == RES_OK);
	assert(madb_int.escribir(valorDos,1) == RES_OK);
	assert(madb_int.get_tamanio_archivo() == 2*sizeof(int));
	assert(madb_int.cerrar_archivo() == RES_OK);

	// Assert
	assert(madb_int.abrir_archivo("madb_int.dat","rb+") == RES_OK);
	assert(madb_int.truncar(1) == RES_OK);
	assert(madb_int.get_tamanio_archivo() == 1*sizeof(int));
	assert(madb_int.leer(NULL,2) == RES_ERROR);

	assert(madb_int.cerrar_archivo() == RES_OK);

	print_test_ok("test_madb_truncar");
}
