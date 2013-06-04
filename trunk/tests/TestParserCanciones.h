/*
 * TestParserCanciones.h
 *
 *  Created on: Jun 4, 2013
 *      Author: maine
 */

#ifndef TESTPARSERCANCIONES_H_
#define TESTPARSERCANCIONES_H_
#include "Test.h"

class TestParserCanciones : public Test {
public:
	TestParserCanciones();
	virtual ~TestParserCanciones();
	void ejecutar();

private:
	void test_leer_de_archivo();
};

#endif /* TESTPARSERCANCIONES_H_ */
