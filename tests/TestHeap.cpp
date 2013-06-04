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

	heap.transformar_en_heap(vectorAOrdenar,4);
//verifico que haya quedado un heap
	assert (heap.comparar_registros_variables(vectorAOrdenar[0],vectorAOrdenar[1])==-1);
	assert (heap.comparar_registros_variables(vectorAOrdenar[0],vectorAOrdenar[2])==-1);
	assert (heap.comparar_registros_variables(vectorAOrdenar[1],vectorAOrdenar[3])==-1);

	print_test_ok("test_heap_ordenar_regs");
}

void TestHeap::test_comparar_reg_variables_string()
{
	Heap heap;


	RegistroVariable rv1,rv2;
	int* campo1;
	int* campo2;
	campo1 = new int;
	campo2 = new int;
	(*campo1) = 1;
	(*campo2) = 2;
	rv1.agregar_campo((char*)campo1,sizeof(int));
	rv2.agregar_campo((char*)campo2,sizeof(int));

	assert (heap.comparar_registros_variables(rv1,rv2)==-1);

	RegistroVariable rv3,rv4;
	rv3.agregar_campo((char*)campo1,sizeof(int));
	rv4.agregar_campo((char*)campo1,sizeof(int));

	assert (heap.comparar_registros_variables(rv3,rv4)==0);


	RegistroVariable rv5,rv6;
	(*campo1) =3;
	(*campo2) =1;
	rv5.agregar_campo((char*)campo1,sizeof(int));
	rv6.agregar_campo((char*)campo2,sizeof(int));

	delete campo1;
	delete campo2;

	assert (heap.comparar_registros_variables(rv5,rv6)==1);

	print_test_ok("test_comparar_reg_variables_string");
}

