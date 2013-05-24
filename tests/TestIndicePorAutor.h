#ifndef TESTINDICEPORAUTOR_H_INCLUDED
#define TESTINDICEPORAUTOR_H_INCLUDED

#include "CapaLogica/Indices/IndiceInvertidoPorAutor.h"
#include "CapaLogica/Indices/ArchivoListas.h"
#include "CapaFisica/ManejadorArchivos.h"
#include "CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "CapaFisica/RegistroVariable.h"
#include "CapaLogica/ManejoArchivos/RegistroClave.h"
#include "CapaLogica/ManejoArchivos/ClaveX.h"
#include "Test.h"

class TestIndicePorAutor : public Test
{
    public:

        TestIndicePorAutor();

        virtual ~TestIndicePorAutor();

        void ejecutar();

    private:

        void test_crear_indice();
        //Crea el indice y verificamos que se creen todos los archivos como corresponde

        void test_agregar_cancion();
        //Agregamos una cancion y verificamos que se guarde correctamente en todos los archivos

        void test_devolver_canciones_por_autor();
        //Probamos que si no se agrego ninguna cancion devuelva NO_EXISTE, luego agregamos una cancion
        //y vemos si la devuelve correctamente y tambien probamos pedirle una cancion que no existe,
        //deberia devolver NO_EXISTE

        void test_borrar_indice();
        //Probamos que se borren todos los archivos que tenian

        void test_agregar_muchas_canciones();
        //Probamos que al agregar muchas canciones en el indice las listas se creen correctamente y
        //vemos si se guarda correctamente los registros de autores como corresponde

        void test_devolver_muchas_canciones_por_autor();
        //Probamos que se devuelvan todas las canciones que correspondan a un autor con muchas canciones
        // y a otro con pocas canciones asociadas a el

    private:

        virtual void eliminar_archivos();

        virtual void crear_reg_cancion(char* autor, RegistroCancion &reg);
};

#endif // TESTINDICEPORAUTOR_H_INCLUDED