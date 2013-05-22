#ifndef ARCHIVOLISTAS_H_INCLUDED
#define ARCHIVOLISTAS_H_INCLUDED

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"

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

        virtual int devolver(RegistroVariable *listaDeCanciones, unsigned short pos_lista);
        //Guarda en la listaDeCanciones la lista que se encuentra en la posicion pos_lista dentro del archivo

        virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID);
        //Agrega el ID a las listas de las posiciones que guarda ref_listas

        virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID, int *pos);
        /****ver si me sirve para indice por frase**/

        virtual int eliminar(std::string directorioSalida, std::string fileNamee);
        //Elimina el archivo con la direccion y el nombre que se le pasan por parametro

        virtual unsigned short get_cantidad_listas();
        //Devuelve la cantidad de listas

};



#endif // ARCHIVOLISTAS_H_INCLUDED
