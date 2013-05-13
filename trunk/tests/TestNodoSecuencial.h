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
		void test_crear();
		void test_insertar_simple();
		void test_empaquetar_desempaquetar();
		void test_insertar_eliminar();
		void test_overflow();
};

#endif /* TESTNODOSECUENCIAL_H_ */
