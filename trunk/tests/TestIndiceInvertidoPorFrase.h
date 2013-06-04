/*
 * TestIndiceInvertidoPorFrase.h
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */


#ifndef TESTINDICEINVERTIDOPORFRASE_H_INCLUDED
#define TESTINDICEINVERTIDOPORFRASE_H_INCLUDED

#include "../src/CapaLogica/Indices/IndiceInvertido.h"
#include "../src/CapaLogica/Indices/ArchivoListas.h"
#include "../src/CapaFisica/ManejadorArchivos.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../src/CapaFisica/RegistroVariable.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveX.h"
#include "Test.h"


class TestIndiceInvertidoPorFrase : public Test
{
    public:

		TestIndiceInvertidoPorFrase();

        virtual ~TestIndiceInvertidoPorFrase();

        void ejecutar();

    private:

        void test_indice_por_frase_agregar_cancion();
        //Agregamos una cancion y verificamos que se guarde correctamente en todos los archivos

        void test_indice_por_frase_devolver_canciones_con_1termino_1cancion();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos una cancion
        //y vemos si la devuelve un termino correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_devolver_canciones_con_1termino_varias_canciones();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos varias cancion
        //y vemos si devuelve un termino correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_devolver_canciones_con_una_cancion();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos una cancion
        //y vemos si la devuelve una frase correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_devolver_canciones_con_varias_canciones();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos varias cancion
        //y vemos si devuelve una frase correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_devolver_canciones_con_una_cancion_terminos_repetidos();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos una cancion
        //y vemos si la devuelve una frase correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_devolver_canciones_con_varias_canciones_terminos_repetidos();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos varias cancion
        //y vemos si devuelve una frase correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_por_frase_borrar_indice();
        //Probamos que se borren todos los archivos que tenian


    private:
        virtual void crear_reg_cancion(std::string letra, RegistroCancion &reg);
};

#endif // TESTINDICEINVERTIDOPORFRASE_H_INCLUDED

