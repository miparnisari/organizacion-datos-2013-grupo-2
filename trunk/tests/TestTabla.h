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
	void test_crear_tabla();

	//pruebo que se duplique la tabla correctamente
	void test_duplicacion();

	//pruebo que se divide la tabla correctamente
	void test_dividir();

	//pruebo que no se puede dividir la tabla si esta tiene dimension 1
	void test_no_se_puede_dividir();

	//pruebo que se modifique un valor de la tabla
	void test_modificar_valor_de_tabla();
};


#endif /* TESTTABLA_H_ */
