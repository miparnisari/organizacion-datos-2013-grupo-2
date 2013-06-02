/*
 * TestOrdenamientoExterno.cpp
 *
 *  Created on: May 16, 2013
 *      Author: juan
 */

#include "TestOrdenamientoExterno.h"

TestOrdenamientoExterno::TestOrdenamientoExterno() {
}

TestOrdenamientoExterno::~TestOrdenamientoExterno() {
}

void TestOrdenamientoExterno::ejecutar(){
//	test_generar_runs(); comento porque tardan mucho en correr.
//	test_merge_runs();
	test_ordenar();

}

void TestOrdenamientoExterno::test_generar_runs(){

	ManejadorRegistrosVariables mv;
	mv.eliminar_archivo("archivoOrdenamiento.dat");
	mv.crear_archivo("archivoOrdenamiento.dat");
	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		string clave="aaa";

		stringstream conversor;
		conversor << clave;
		string campoClave= conversor.str();

		RegistroVariable regVariable;

		regVariable.agregar_campo(campoClave.c_str(),campoClave.length());

		mv.agregar_registro(& regVariable);
	}

	SortExterno ordenador("archivoOrdenamiento.dat");

	ordenador._generar_runs();

	std::vector<string> runs = ordenador._getVector();

	Heap heap;

	for (unsigned short i= 0; i< runs.size(); i++)
	{

		ManejadorRegistrosVariables mv2;
		assert(mv2.abrir_archivo(runs[i])==RES_OK);

		for (int j = 0; j < mv2.get_cantidad_registros_ocupados()-1; j++)
		{
			RegistroVariable rv1,rv2;

			mv2.get_registro_ocupado(&rv1,j);
			mv2.get_registro_ocupado(&rv2,j+1);

			assert(heap.comparar_registros_variables(rv1,rv2)<=0);
		}
		mv2.eliminar_archivo(runs[i]);
	}

	print_test_ok("test_generar_runs");
	mv.eliminar_archivo("archivoOrdenamiento.dat");

}

void TestOrdenamientoExterno::test_merge_runs(){

	ManejadorRegistrosVariables mv;
	mv.eliminar_archivo("archivoOrdenamiento.dat");
	mv.crear_archivo("archivoOrdenamiento.dat");
	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		string clave="aaa";
		clave[0] = (char) ((rand() % 20) + 65);

		stringstream conversor;
		conversor << clave;
		string campoClave= conversor.str();

		RegistroVariable regVariable;

		regVariable.agregar_campo(campoClave.c_str(),campoClave.length());

		mv.agregar_registro(& regVariable);
	}

	SortExterno ordenador("archivoOrdenamiento.dat");

	ordenador._generar_runs();

	ordenador._merge();

	Heap heap;

	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroVariable rv1,rv2;

		mv.get_registro_ocupado(&rv1,i);
		mv.get_registro_ocupado(&rv2,i+1);

		assert(heap.comparar_registros_variables(rv1,rv2)<=0);
	}

	print_test_ok("test_merge_runs");
}

void TestOrdenamientoExterno::test_ordenar(){

	ManejadorRegistrosVariables mv;
	mv.eliminar_archivo("archivoOrdenamiento.dat");
	mv.crear_archivo("archivoOrdenamiento.dat");
	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		//agrego clave en el primer campo
		string clave="aaa";
		clave[0] = (char) ((rand() % 20) + 65);

		stringstream conversor;
		conversor << clave;
		string campoClave= conversor.str();

		RegistroVariable regVariable;

		regVariable.agregar_campo(campoClave.c_str(),campoClave.length());

		//agrego clave en el segundo campo
		string clave2="aaa";
		clave2[0] = (char) ((rand() % 20) + 65);

		conversor << clave2;
		string campoClave2= conversor.str();

		regVariable.agregar_campo(campoClave2.c_str(),campoClave2.length());

		mv.agregar_registro(& regVariable);
	}

	SortExterno ordenador("archivoOrdenamiento.dat");

	ordenador.ordenar_archivo();

	Heap heap;

	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroVariable rv1,rv2;

		mv.get_registro_ocupado(&rv1,i);
		mv.get_registro_ocupado(&rv2,i+1);

		assert(heap.comparar_registros_variables(rv1,rv2)<=0);
	}

	print_test_ok("test_ordenar");
}
