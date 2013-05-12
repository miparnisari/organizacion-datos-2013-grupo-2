/*
 * TestHeap.cpp
 *
 *  Created on: May 11, 2013
 *      Author: martin
 */

#include "TestHeap.h"

TestHeap::TestHeap()
	: Test()
{
}

TestHeap::~TestHeap()
{
}

void TestHeap::ejecutar()
{
	testHeapsort();
}


void TestHeap::testHeapsort()
{

	Heap heap;

	ClaveX clave1;
	clave1.set_clave("aa");

	ClaveX clave2;
	clave2.set_clave("bb");

	ClaveX clave3;
	clave2.set_clave("cc");

	RegistroClave registro1;
	RegistroClave registro2;
	RegistroClave registro3;

	registro1.set_clave(clave1);
	registro2.set_clave(clave2);
	registro3.set_clave(clave3);

	std::string campo="campo_reg_1";
	registro1.agregar_campo(campo.c_str(),campo.size());

	campo="campo_reg_2";
	registro2.agregar_campo(campo.c_str(),campo.size());

	campo="campo_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());

	RegistroClave vectorAOrdenar[3];
	vectorAOrdenar[0]=registro1;
	vectorAOrdenar[1]=registro3;
	vectorAOrdenar[2]=registro2;

	heap.ordenar(vectorAOrdenar,3);

	assert(vectorAOrdenar[0]<=vectorAOrdenar[1]);
	assert(vectorAOrdenar[1]<=vectorAOrdenar[2]);
}

