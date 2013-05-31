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
	test_comparar_reg_variables_string();
	test_ordenar_registros();
}

void TestHeap::test_ordenar_registros()
{

	Heap heap;

	RegistroVariable registro1;
	RegistroVariable registro2;
	RegistroVariable registro3;
	RegistroVariable registro4;

	std::string campo="aa";
	registro1.agregar_campo(campo.c_str(),campo.size());

	campo="bb";
	registro2.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_2";
	registro2.agregar_campo(campo.c_str(),campo.size());

	campo="cc";
	registro3.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());
	campo="campo3_reg_3";
	registro3.agregar_campo(campo.c_str(),campo.size());

	campo="bf";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo2_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo3_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());
	campo="campo4_reg_4";
	registro4.agregar_campo(campo.c_str(),campo.size());

	RegistroVariable vectorAOrdenar[4];
	vectorAOrdenar[0] = registro4;
	vectorAOrdenar[1] = registro3;
	vectorAOrdenar[2] = registro1;
	vectorAOrdenar[3] = registro2;

	heap.ordenar(vectorAOrdenar,4);

	assert (heap.comparar_registros_variables(vectorAOrdenar[0],vectorAOrdenar[1])==-1);
	assert (heap.comparar_registros_variables(vectorAOrdenar[1],vectorAOrdenar[2])==-1);
	assert (heap.comparar_registros_variables(vectorAOrdenar[2],vectorAOrdenar[3])==-1);

	print_test_ok("test_heap_ordenar_regs");
}

void TestHeap::test_comparar_reg_variables_string()
{
	Heap heap;


	RegistroVariable rv1,rv2;
	string campo1,campo2;
	campo1="aaa";
	campo2="bbb";
	rv1.agregar_campo(campo1.c_str(),campo1.length());
	rv2.agregar_campo(campo2.c_str(),campo2.length());

	assert (heap.comparar_registros_variables(rv1,rv2)==-1);


	RegistroVariable rv3,rv4;
	campo1="aaa";
	rv3.agregar_campo(campo1.c_str(),campo1.length());
	rv4.agregar_campo(campo1.c_str(),campo1.length());

	assert (heap.comparar_registros_variables(rv3,rv4)==0);


	RegistroVariable rv5,rv6;
	campo1="ccc";
	campo2="aaa";
	rv5.agregar_campo(campo1.c_str(),campo1.length());
	rv6.agregar_campo(campo2.c_str(),campo2.length());

	assert (heap.comparar_registros_variables(rv5,rv6)==1);

	print_test_ok("test_comparar_reg_variables_string");
}

