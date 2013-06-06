#ifndef INDICEINVERTIDOPORAUTOR_H_INCLUDED
#define INDICEINVERTIDOPORAUTOR_H_INCLUDED

#include "../../CapaFisica/RegistroVariable.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../Constantes.h"
#include "../../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "ArchivoListas.h"

class IndiceInvertidoPorAutor
{
    private:

        std::string ruta;

        ArbolBMas indice;

        ArchivoListas listas;

    public:

        IndiceInvertidoPorAutor();

        virtual ~IndiceInvertidoPorAutor();

        virtual int crear_indice(std::string directorioSalida);
        //Crea el indice en la direccion que se le pasan por parametro

        virtual int abrir_indice(std::string directorioSalida);
        //Abre el indice en la direccion que se le pasan por parametro

        virtual int agregar_cancion(RegistroCancion & cancion, int IDcancion);
        //Agrega la cancion a la lista de canciones de cada autor

        virtual long buscar_autor(std::string autor, RegistroVariable &listaDeCanciones);
        //Guarda en la listaDeCanciones la lista de canciones del autor buscado

        virtual int borrar_indice();
        //Borra el indice junto a sus archivos

};

#endif // INDICEINVERTIDOPORAUTOR_H_INCLUDED
