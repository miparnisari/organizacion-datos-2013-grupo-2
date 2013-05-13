/*
 * TestManejadorBloques.h
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#ifndef TESTMANEJADORBLOQUES_H_
#define TESTMANEJADORBLOQUES_H_

#include "Test.h"

class TestManejadorBloques: public Test {
	public:
		TestManejadorBloques();
		virtual ~TestManejadorBloques();
		void ejecutar();

	private:
		void test_manejador_bloques_crear();
		void test_manejador_bloques_escribir_bloques();
		void test_manejador_bloques_masivo();
		void test_eliminar();
};

#endif /* TESTMANEJADORBLOQUES_H_ */
