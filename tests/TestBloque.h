/*
 * TestBloque.h
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#ifndef TESTBLOQUE_H_
#define TESTBLOQUE_H_

#include "Test.h"

class TestBloque: public Test {
	public:
		TestBloque();
		virtual ~TestBloque();
		void ejecutar();

	private:
		void test_agregar_registros();
		void test_remover_registros();
		void test_recuperar_registros();
		void test_empaquetar_desempaquetar();
		void test_eliminar_bloque();
};

#endif /* TESTBLOQUE_H_ */
