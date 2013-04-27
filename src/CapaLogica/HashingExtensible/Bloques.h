#ifndef BLOQUES_H_INCLUDED
#define BLOQUES_H_INCLUDED

#include "../../CapaFisica/RegistroVariable.h"
#include "../../Constantes.h"
#include "../../CapaFisica/Bloque.h"

class Bloques
{
  /*  private:

        int tamBloque;

        lista bloquesLibres;

        string fileName;

        int cant;

        int bloquesReservados*/

    public:

        Bloques(int tamBloque, int bloquesReservados, std::string fileName);

        virtual ~Bloques();

        virtual int agregar_bloque(Bloque bloque);
        //Agrega el bloque al archivo de bloques y devuelve la posicion relativa del bloque dentro del archivo

        virtual int devolver_bloque(int posicion, Bloque *bloqueDestino);
        //Se guarda el bloque de la posicion que se paso por parametro en el parametro bloqueDestino,
        //si la posicion esta vacia entonces se devuelve NO_EXISTE sino devuelve RES_OK

        virtual int eliminar_bloque(int posicion);
        //Se elimina el bloque de la posicion pasada por parametro dentro del archivo

};

/*Esta clase fue creada con la responsabilidad de manejar los bloques dentro del archivo*/

#endif // BLOQUES_H_INCLUDED
