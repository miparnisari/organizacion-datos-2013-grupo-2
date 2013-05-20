#ifndef TESTHASHINGEXTENSIBLE_H_
#define TESTHASHINGEXTENSIBLE_H_

#include "Test.h"

class TestHashingExtensible : public Test {

        public:

                TestHashingExtensible();

                virtual ~TestHashingExtensible();

                void ejecutar();

        private:

                void test_hashing_guardar_y_leer_int();

                void test_crear_hashing();
                //Probamos que se cree la tabla y el primer bloque de registros cuando se cree el hashing

                void test_eliminar_registro();
        //Probamos eliminar un registro y ver si nos devuelve algo al buscarlo

                void test_agregar_y_devolver_registro();
                //Probamos que se agregue y se devuelva correctamente los datos

                void test_crear_hashing_cerrarlo_y_abrirlo();
                //Probamos de guardar un registro en el hash, lo cerramos y vemos si al abrirlo sigue estando

                void test_agregar_varios_registros_y_devolver();
                //Probamos que se agregue y se devuelva correctamente los datos

                void test_agregar_reg_y_duplicar_tabla();
                //Probamos llenando el primer bloque para ver si se duplica la tabla y si se crearon solo dos bloques

                void test_eliminar_reg_y_dividir_tabla();
                //Probamos llenando el primer bloque para ver si al eliminar un registro la tabla se divide y se borra un bloque

                /**Son para el funcionamiento interno del test*/

                int crear_registro_y_agregar(HashingExtensible *hash1, std::string campo, ClaveX clave);

                void recuperar_dato_registro(char *campoRecuperado, ClaveX clave, HashingExtensible hash1);

};

#endif /* TESTHASHINGEXTENSIBLE_H_ */
