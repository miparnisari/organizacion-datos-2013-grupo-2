/*
 * TestHashingExtensible.h
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#ifndef TESTHASHINGEXTENSIBLE_H_
#define TESTHASHINGEXTENSIBLE_H_

#include "Test.h"

class TestHashingExtensible : public Test {

	public:
		TestHashingExtensible();
		virtual ~TestHashingExtensible();
		void ejecutar();

	private:
		void eliminar_archivo(std::string direccion);
		void test_crear_hashing();
		void test_eliminar_registro();
		void test_agregar_y_devolver_registro();
		void test_crear_hashing_cerrarlo_y_abrirlo();
		void test_agregar_reg_y_duplicar_tabla();
};

#endif /* TESTHASHINGEXTENSIBLE_H_ */
