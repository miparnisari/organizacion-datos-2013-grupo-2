/*
 * TestHeap.cpp
 *
 *  Created on: May 11, 2013
 *      Author: martin
 */

#include "TestHeap.h"

TestHeap::TestHeap()
{
}

TestHeap::~TestHeap()
{
}

void TestHeap::ejecutar()
{
	test_ordenar_registros();
}

void TestHeap::test_ordenar_registros()
{

	Heap heap;

	ClaveX clave1;
	clave1.set_clave("aa");

	ClaveX clave2;
	clave2.set_clave("bb");

	ClaveX clave3;
	clave3.set_clave("cc");

	ClaveX clave4;
	clave4.set_clave("bf");

	RegistroClave registro1;
	RegistroClave registro2;
	RegistroClave registro3;
	RegistroClave registro4;

	registro1.set_clave(clave1);
	registro2.set_clave(clave2);
	registro3.set_clave(clave3);
	registro4.set_clave(clave4);

	std::string campo="campo1_reg_1";
	registro1.agregar_campo(campo.c_str(),campo.size());

	campo="campo1_reg_2";
	registro2.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_2";
	registro2.agregar_campo(campo.c_str(),campo.size());

	campo="campo1_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());
	campo="campo3_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());

	campo="campo1_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo3_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo4_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());


	RegistroClave vectorAOrdenar[4];
	vectorAOrdenar[0]=registro4;
	vectorAOrdenar[1]=registro3;
	vectorAOrdenar[2]=registro1;
	vectorAOrdenar[3]=registro2;

	heap.ordenar(vectorAOrdenar,4);

	assert(vectorAOrdenar[0]<=vectorAOrdenar[1]);
	assert(vectorAOrdenar[1]<=vectorAOrdenar[2]);
	assert(vectorAOrdenar[2]<=vectorAOrdenar[3]);

	print_test_ok("test_heap_ordenar_regs");
}

