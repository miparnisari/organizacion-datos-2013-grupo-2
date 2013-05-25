/*
 * IndiceInvertidoPorTitulo.h
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */

#ifndef TESTINDICEPORTITULO_H_INCLUDED
#define TESTINDICEPORTITULO_H_INCLUDED

#include "../src/CapaLogica/Indices/IndiceInvertidoPorTitulo.h"
#include "../src/CapaLogica/Indices/ArchivoListas.h"
#include "../src/CapaFisica/ManejadorArchivos.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../src/CapaFisica/RegistroVariable.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveX.h"
#include "Test.h"

class TestIndicePorTitulo : public Test
{
    public:

	TestIndicePorTitulo();

        virtual ~TestIndicePorTitulo();

        void ejecutar();

    private:

        void test_indice_titulo_crear_indice();
        //Crea el indice y verificamos que se creen todos los archivos como corresponde

        void test_indice_titulo_agregar_cancion();
        //Agregamos una cancion y verificamos que se guarde correctamente en todos los archivos

        void test_indice_titulo_devolver_canciones_por_autor();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos una cancion
        //y vemos si la devuelve correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_indice_titulo_borrar_indice();
        //Probamos que se borren todos los archivos que tenian

        void test_indice_titulo_agregar_muchas_canciones();
        //Probamos que al agregar muchas canciones en el indice las listas se creen correctamente y
        //vemos si se guarda correctamente los registros de titulos como corresponde

        void test_indice_titulo_devolver_muchas_canciones_por_autor();
        //Probamos que se devuelvan todas las canciones que correspondan a un titulo con muchas canciones
        // y a otro con pocas canciones asociadas a el

    private:

        virtual void eliminar_archivos();

        virtual void crear_reg_cancion(const char* titulo, RegistroCancion &reg);
};

#endif // TESTINDICEPORTITULO_H_INCLUDED


