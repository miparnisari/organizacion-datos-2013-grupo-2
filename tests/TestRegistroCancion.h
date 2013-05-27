/*
 * TestRegistroCancion.h
 *
 *  Created on: May 13, 2013
 *      Author: maine
 */

#ifndef TESTREGISTROCANCION_H_
#define TESTREGISTROCANCION_H_

#include "Test.h"

class TestRegistroCancion : public Test {
	public:
		TestRegistroCancion();
		virtual ~TestRegistroCancion();
		void ejecutar();

	private:
		void test_parametros();
		void test_obtener_letras_parseadas();
		void guardar_en_archivo_registros_variables();
};

#endif /* TESTREGISTROCANCION_H_ */
