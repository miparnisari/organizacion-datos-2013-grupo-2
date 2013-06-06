#ifndef INDICEINVERTIDOPORTITULO_H_INCLUDED
#define INDICEINVERTIDOPORTITULO_H_INCLUDED

#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../Constantes.h"
#include "../../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "ArchivoListas.h"

class IndiceInvertidoPorTitulo
{
    private:

        std::string ruta;

        HashingExtensible indice;

        ArchivoListas listas;

    public:

        IndiceInvertidoPorTitulo();

        virtual ~IndiceInvertidoPorTitulo();

        virtual int crear_indice(std::string directorioSalida);
        //Crea el indice en la direccion que se le pasan por parametro

        virtual int abrir_indice(std::string directorioSalida);
        //Abre el indice en la direccion que se le pasan por parametro

        virtual int agregar_cancion(RegistroCancion & cancion, int IDcancion);
        //Agrega el IDcancion a la lista de canciones que tienen el mismo titulo

        virtual long buscar_titulo(std::string titulo, RegistroVariable &listaDeCanciones);
        //Guarda en la listaDeCanciones la lista de canciones que tienen el mismo titulo buscado

        virtual int borrar_indice();
        //Borra el indice junto a sus archivos

};

#endif // INDICEINVERTIDOPORTITULO_H_INCLUDED
