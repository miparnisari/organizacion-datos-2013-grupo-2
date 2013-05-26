/*
 * TestArchivoListas.h
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */

#ifndef TESTARCHIVOLISTAS_H_
#define TESTARCHIVOLISTAS_H_

#include "Test.h"


class TestArchivoListas : public Test {

        public:

				TestArchivoListas();

                ~TestArchivoListas();

                void ejecutar();

        private:

                virtual void test_archivo_listas_crear();
                //Verifico que se cree el archivo y que este vacio

                virtual void test_archivo_listas_eliminar();
                //Verifico que se borre el archivo

                virtual void test_archivo_listas_agregar();
                //Verifico que se agregue la lista tal cual a como se ingreso

                virtual void test_archivo_listas_devolver();
                //Verifico que se devuelva la lista tal cual a como se ingreso

                virtual void test_archivo_listas_reconstruir_listas();
                //Verifico que se reconstruya la lista como se deberia

                virtual void test_archivo_listas_reconstruir_listas_por_frase();
                //Verifico que se reconstruya la lista como se deberia

                virtual void eliminar_archivos();

};

#endif // TESTARCHIVOLISTAS_H_


