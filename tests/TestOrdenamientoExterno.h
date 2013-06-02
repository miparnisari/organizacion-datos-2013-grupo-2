/*
 * TestOrdenamientoExterno.h
 *
 *  Created on: May 16, 2013
 *      Author: juan
 */

#ifndef TESTORDENAMIENTOEXTERNO_H_
#define TESTORDENAMIENTOEXTERNO_H_

#include "Test.h"

class TestOrdenamientoExterno: public Test {
public:
	TestOrdenamientoExterno();
	virtual ~TestOrdenamientoExterno();

	void ejecutar();

	void test_ordenar();
	void test_generar_runs();
	void test_merge_runs();
};

#endif /* TESTORDENAMIENTOEXTERNO_H_ */
