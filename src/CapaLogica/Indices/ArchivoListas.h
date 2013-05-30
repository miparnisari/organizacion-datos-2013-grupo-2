#ifndef ARCHIVOLISTAS_H_INCLUDED
#define ARCHIVOLISTAS_H_INCLUDED

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"
#include <stdio.h>

class ArchivoListas
{
    private:

        std::string fileName;

        std::string directorioSalida;

        ManejadorRegistrosVariables archivo;

    public:

        ArchivoListas();

        virtual ~ArchivoListas();

        virtual int crear(std::string directorioSalida, std::string fileNamee);
        //Crea el archivo con la direccion y el nombre que se le pasan por parametro

        virtual int abrir(std::string directorioSalidaa, std::string fileNamee);
        //Abre el archivo con la direccion y el nombre que se le pasan por parametro

        virtual short agregar(RegistroVariable *listaDeCanciones);
        //Agrega una lista al archivo y devuelve el offset de este

        virtual int devolver(RegistroVariable *listaDeCanciones, unsigned short pos_lista);
        //Guarda en la listaDeCanciones la lista que se encuentra en la posicion pos_lista dentro del archivo

        virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID);
        //Agrega el ID a las listas de las posiciones que guarda ref_listas. Se penso para los indices por Titulo y Autor

        virtual int recontruir_listas(unsigned short ref_lista, RegistroVariable &listaModificada);
        //Modifico la lista que se pasa por referencia. Se penso para el indice invertido por frase

        virtual int eliminar(std::string directorioSalida, std::string fileNamee);
        //Elimina el archivo con la direccion y el nombre que se le pasan por parametro

        virtual int get_cantidad_listas();
        //Devuelve la cantidad de listas

};



#endif // ARCHIVOLISTAS_H_INCLUDED
