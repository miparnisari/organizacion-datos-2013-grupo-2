/*
 * TestNodoInterno.h
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#ifndef TESTNODOINTERNO_H_
#define TESTNODOINTERNO_H_

#include "Test.h"

class TestNodoInterno : public Test
{

	public:
		TestNodoInterno();
		virtual ~TestNodoInterno();
		void ejecutar();
		void todo_tipo_empaquetado();


	private:
		void crear_nodo_con_demasiadas_claves_falla();
		void insertar_hijos();
};

#endif /* TESTNODOINTERNO_H_ */
