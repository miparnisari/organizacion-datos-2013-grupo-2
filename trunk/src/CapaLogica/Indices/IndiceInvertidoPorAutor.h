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
        virtual int abrir_indice(std::string directorioSalida);

        virtual int agregar_cancion(RegistroCancion cancion, int IDcancion);
        virtual long buscar_autor(std::string autor, RegistroVariable &listaDeCanciones);
        virtual int borrar_indice();

    private:
    //    virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int IDcancion);
};

#endif // INDICEINVERTIDOPORAUTOR_H_INCLUDED
