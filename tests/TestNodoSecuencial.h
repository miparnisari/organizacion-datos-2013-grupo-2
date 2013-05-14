/*
 * TestNodoSecuencial.h
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#ifndef TESTNODOSECUENCIAL_H_
#define TESTNODOSECUENCIAL_H_

#include "Test.h"

class TestNodoSecuencial: public Test {
	public:
		TestNodoSecuencial();
		virtual ~TestNodoSecuencial();
		void ejecutar();

	private:
		void test_nodo_sec_crear();
		void test_nodo_sec_insertar_simple();
		void test_nodo_sec_empaquetar_desempaquetar();
		void test_nodo_sec_insertar_eliminar();
		void test_nodo_sec_overflow();
};

#endif /* TESTNODOSECUENCIAL_H_ */