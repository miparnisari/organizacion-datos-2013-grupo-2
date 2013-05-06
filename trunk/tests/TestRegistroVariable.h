/*
 * TestRegistroVariable.h
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#ifndef TESTREGISTROVARIABLE_H_
#define TESTREGISTROVARIABLE_H_

#include "Test.h"

class TestRegistroVariable: public Test {
	public:
		TestRegistroVariable();
		virtual ~TestRegistroVariable();
		void ejecutar();

	private:
		void test_agregar_campos();
		void test_eliminar();
		void test_empaquetar_desempaquetar();
		void test_recuperar_campos();
};

#endif /* TESTREGISTROVARIABLE_H_ */
