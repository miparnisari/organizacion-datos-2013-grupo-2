/*
 * TestClave.h
 *
 *  Created on: Jun 4, 2013
 *      Author: maine
 */

#ifndef TESTCLAVE_H_
#define TESTCLAVE_H_

#include "Test.h"

class TestClave: public Test {
public:
	TestClave();
	virtual ~TestClave();
	void ejecutar();

private:
	bool mayor_a(Clave* a,Clave* b);

	void test_clave_string();
	void test_clave_numerica();
	void test_vector_clavex();
	void test_clavex();
};

#endif /* TESTCLAVE_H_ */
