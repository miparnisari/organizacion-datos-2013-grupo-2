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
//	test_generar_runs();
	test_merge_runs();
}

void TestOrdenamientoExterno::test_generar_runs(){

	ManejadorRegistrosVariables mv;
	mv.crear_archivo("archivoOrdenamiento.dat");

	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		//srand (time(NULL));
		string clave="aaa";
//		clave[0] = (char) ((rand() % 20) + 65);

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

//			cout<<"num del run: "<<i<<endl;
//
//			cout<<"claves del run: "<<endl; //todo
//
//			char* campo=new char[rv1.get_tamanio_campo(0)+1];
//			rv1.recuperar_campo(campo,0);
//			campo[rv1.get_tamanio_campo(0)]='\0';
//			string campoImprimir1 = campo;
//			delete campo;
//
//			campo=new char[rv2.get_tamanio_campo(0)+1];
//			rv2.recuperar_campo(campo,0);
//			campo[rv2.get_tamanio_campo(0)]='\0';
//			string campoImprimir2 = campo;
//			delete campo;
//
//			cout<<campoImprimir1<<endl;
//			cout<<campoImprimir2<<endl;

			assert(heap.comparar_registros_variables(rv1,rv2)<=0);
		}
		mv2.eliminar_archivo(runs[i]);
	}

	print_test_ok("test_generar_runs");
	mv.eliminar_archivo("archivoOrdenamiento.dat");

}

void TestOrdenamientoExterno::test_merge_runs(){

	ManejadorRegistrosVariables mv;
	mv.crear_archivo("archivoOrdenamiento.dat");

	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		//srand (time(NULL));
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
