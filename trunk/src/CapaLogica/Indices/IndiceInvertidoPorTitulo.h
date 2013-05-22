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
        virtual int abrir_indice(std::string directorioSalida);

        virtual int agregar_cancion(RegistroCancion cancion, int IDcancion);
        virtual long buscar_titulo(std::string titulo, RegistroVariable &listaDeCanciones);
        virtual int borrar_indice();
};

#endif // INDICEINVERTIDOPORTITULO_H_INCLUDED
