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
		void prueba();
};

#endif /* TESTREGISTROCANCION_H_ */
