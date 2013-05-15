/*
 * TestManejadorArchivoDatosBasicos.h
 *
 *  Created on: May 15, 2013
 *      Author: maine
 */

#ifndef TESTMANEJADORARCHIVODATOSBASICOS_H_
#define TESTMANEJADORARCHIVODATOSBASICOS_H_

#include "Test.h"

class TestManejadorArchivoDatosBasicos: public Test {
	public:
		TestManejadorArchivoDatosBasicos();
		virtual ~TestManejadorArchivoDatosBasicos();
		void ejecutar();

	private:
		void test_madb_leer();
};

#endif /* TESTMANEJADORARCHIVODATOSBASICOS_H_ */
