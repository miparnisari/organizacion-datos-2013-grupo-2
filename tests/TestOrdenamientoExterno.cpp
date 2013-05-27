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
	test_generar_runs();
	test_merge_runs();
}

void TestOrdenamientoExterno::test_generar_runs(){

	ManejadorRegistrosVariables mv;
	mv.crear_archivo("archivoOrdenamiento.dat");

	mv.abrir_archivo("archivoOrdenamiento.dat");

	while (mv.get_tamanio_archivo()<1024*10) //10kB
	{
		ClaveX clave;
		//srand (time(NULL));
		clave.set_clave( (rand() % 100) );
		RegistroClave regClave;
		regClave.set_clave(clave);

		mv.agregar_registro(& regClave);
	}

	SortExterno ordenador("archivoOrdenamiento.dat");

	ordenador._generar_runs();

	std::vector<string> runs = ordenador._getVector();

	for (unsigned short i= 0; i< runs.size(); i++)
	{

		ManejadorRegistrosVariables mv2;
		assert(mv2.abrir_archivo(runs[i])==RES_OK);

		for (int j = 0; j < mv2.get_cantidad_registros_ocupados()-1; j++)
		{
			RegistroClave regClave1,regClave2;
			ClaveX clave1,clave2;

			mv2.get_registro_ocupado(&regClave1,j);
			mv2.get_registro_ocupado(&regClave2,j+1);

			clave1= regClave1.get_clave();
			clave2= regClave2.get_clave();

//			cout<<"num del run: "<<i<<endl;
//
//			cout<<"claves del run: "<<endl; //todo
//			clave1.imprimir_dato();
//			std::cout<<endl;

			assert (clave1 <= clave2);
		}
	}

	print_test_ok("test_generar_runs");

}

//uso el mismo archivo del test anterior, bien pueden pedirme ordenar lo mismo varias veces
void TestOrdenamientoExterno::test_merge_runs(){

	SortExterno ordenador("archivoOrdenamiento.dat");

	ordenador._generar_runs();

	ordenador._merge();

	ManejadorRegistrosVariables mv;

	mv.abrir_archivo("archivoOrdenamiento.dat");

	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroClave regClave1,regClave2;
		ClaveX clave1,clave2;

		mv.get_registro_ocupado(&regClave1,i);
		mv.get_registro_ocupado(&regClave2,i+1);

		clave1= regClave1.get_clave();
		clave2= regClave2.get_clave();

//		cout<<"ASSERT: "<<endl; //todo
//		clave1.imprimir_dato();
//		std::cout<<endl;
//		clave2.imprimir_dato();
//		std::cout<<endl;

		assert (clave1 <= clave2);
	}

	print_test_ok("test_merge_runs");
}
