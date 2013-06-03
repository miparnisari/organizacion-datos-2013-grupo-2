/*
 * TestNormalizador.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: martin
 */

#include "TestNormalizador.h"

TestNormalizador::TestNormalizador(){

}

TestNormalizador::~TestNormalizador(){

}

void TestNormalizador::ejecutar(){

	this->test_normalizar_input();

}

void TestNormalizador::ingresar_linea(string& linea){

	cout<<">ingrese linea: ";
	cin>>linea;
	cout<<"endl";

}


void TestNormalizador::pausa(){

	int pausa;
	cout<<">ingrese tecla seguido de ENTER para continuar ..."<<endl;
	cin>>pausa;

}


void TestNormalizador::test_normalizar_input(){

	Normalizador n;

	string lineaLarga= "ésta-línea-se-ásemeja-múcho-A\nUNA CANCION\n";
	IMPRIMIR_VARIABLE(lineaLarga);
	IMPRIMIR_VARIABLE(lineaLarga.length());
	string lineaLargaNormalizada;
	lineaLargaNormalizada= n.normalizar_input(lineaLarga);
	IMPRIMIR_VARIABLE(lineaLargaNormalizada);

}
