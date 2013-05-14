/*
 * Test.h
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#ifndef TEST_H_
#define TEST_H_

#include <assert.h>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <climits>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../src/CapaFisica/ManejadorArchivosTexto.h"
#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../src/CapaFisica/Bloque.h"
#include "../src/CapaFisica/ManejadorBloques.h"

#include "../src/CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../src/CapaLogica/HashingExtensible/Tabla.h"
#include "../src/CapaLogica/Parser/ParserCanciones.h"
#include "../src/CapaLogica/ArbolBMas/NodoInterno.h"
#include "../src/CapaLogica/ArbolBMas/NodoSecuencial.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveString.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveNumerica.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveX.h"
#include "../src/Utilitarios/Heap.h"

#include "../src/Constantes.h"

using namespace std;

#define IMPRIMIR_VARIABLE(var)\
	std::cout<<#var<<" = "<<var<<'\n';

class Test {

	public:
		Test();
		virtual ~Test();
		virtual void ejecutar() = 0;
		void print_test_ok(std::string nombreTest);
};

#endif /* TEST_H_ */
