/*
 * TestArbol.h
 *
 *  Created on: May 16, 2013
 *      Author: martin
 */

#ifndef TESTARBOL_H_
#define TESTARBOL_H_

#include "Test.h"


class TestArbol:public Test {

	private:
		void test_arbol_buscar();
		void test_insertar_pocos_registros();
		void test_arbol_abrir_cerrar();
		void test_arbol_abrir_no_existente();
		void test_arbol_insertar_un_registro();
		void test_split_nodo_secuencial();
		void test_split_raiz();

	public:
		void ejecutar();

};

#endif /* TESTARBOL_H_ */
