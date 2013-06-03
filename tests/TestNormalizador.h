/*
 * TestNormalizador.h
 *
 *  Created on: Jun 3, 2013
 *      Author: martin
 */

#ifndef TESTNORMALIZADOR_H_
#define TESTNORMALIZADOR_H_

#include "Test.h"

class TestNormalizador: public Test {

	public:
		void ejecutar();
		TestNormalizador();
		~TestNormalizador();

		static void ingresar_linea(string& linea);
		static void pausa();

	private:
		void test_normalizar_input();


};

#endif /* TESTNORMALIZADOR_H_ */
