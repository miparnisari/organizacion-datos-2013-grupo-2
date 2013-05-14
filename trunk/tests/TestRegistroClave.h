/*
 * TestRegistroClave.h
 *
 *  Created on: May 12, 2013
 *      Author: maine
 */

#ifndef TESTREGISTROCLAVE_H_
#define TESTREGISTROCLAVE_H_

#include "Test.h"

class TestRegistroClave : public Test{
	public:
		TestRegistroClave();
		virtual ~TestRegistroClave();
		void ejecutar();

	private:
		void test_registro_clave_guardar_y_leer();
		void test_registro_clave_empaquetar_desempaquetar();
};

#endif /* TESTREGISTROCLAVE_H_ */
