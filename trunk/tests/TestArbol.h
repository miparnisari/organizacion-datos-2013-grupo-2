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

	public:
		void ejecutar();
		void test_insertar_pocos_registros();
		void test_arbol_abrir_cerrar();
		void test_arbol_abrir_no_existente();
		void test_arbol_insertar_un_registro();

};

#endif /* TESTARBOL_H_ */
