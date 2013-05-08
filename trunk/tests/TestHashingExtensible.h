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
		void test_agregar_varios_registros_y_devolver();
		//Son para el funcionamiento de los test
		void crear_registro_y_agregar(HashingExtensible *hash1, std::string campo, ClaveX clave);
		void recuperar_dato_registro(char *campoRecuperado, ClaveX clave, HashingExtensible hash1);
};

#endif /* TESTHASHINGEXTENSIBLE_H_ */
