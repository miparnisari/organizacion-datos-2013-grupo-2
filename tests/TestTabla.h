/*
 * TestTabla.h
 *
 *  Created on: May 12, 2013
 *      Author: nico
 */

#ifndef TESTTABLA_H_
#define TESTTABLA_H_
#include "../src/CapaLogica/HashingExtensible/Tabla.h"
#include "Test.h"


using namespace std;

class TestTabla : public Test{
public:
	TestTabla();
	virtual ~TestTabla();

	void ejecutar();

private:

	//pruebo que se cree la tabla correctamente
	void test_tabla_hash_crear();

	//pruebo que se duplique la tabla correctamente
	void test_tabla_hash_duplicacion();

	//pruebo que se divide la tabla correctamente
	void test_tabla_hash_dividir();

	//pruebo que no se puede dividir la tabla si esta tiene dimension 1
	void test_no_se_puede_dividir();

	//pruebo que se modifique un valor de la tabla
	void test_tabla_hash_modificar_valor();

	void test_tabla_hash_eliminar_tabla();

	void test_tabla_hash_posiciones_correctas_al_duplicar();

	void test_tabla_hash_posiciones_correctas_al_dividir();

	void test_tabla_hash_integracion();
};


#endif /* TESTTABLA_H_ */
