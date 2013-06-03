/*
 * TestManejadorRegistrosVariables.h
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#ifndef TESTMANEJADORREGISTROSVARIABLES_H_
#define TESTMANEJADORREGISTROSVARIABLES_H_

#include "Test.h"

class TestManejadorRegistrosVariables: public Test {

	public:
		TestManejadorRegistrosVariables();
		virtual ~TestManejadorRegistrosVariables();
		void ejecutar();

	private:
		void test_recuperar_espacio_libre();
		void test_eliminar_por_offset();
		void test_chequear_registros_ocupados();
		void test_masivo();
		void test_contar_registros();
		void test_recuperar_por_offset();
		void test_registro_clave();
		void test_refactorizar();
};

#endif /* TESTMANEJADORREGISTROSVARIABLES_H_ */
