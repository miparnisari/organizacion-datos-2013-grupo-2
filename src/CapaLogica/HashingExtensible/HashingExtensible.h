#ifndef HASHINGEXTENSIBLE_H
#define HASHINGEXTENSIBLE_H

#include "Tabla.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../Constantes.h"

class HashingExtensible
{
    /*Atributos.....
    private:

        string fileName;
        Tabla tabla;
        Bloques bloques;
        int cant_bloques;
        lista bloques_libres;   //manejo de bloques libres
    */

	public:

        HashingExtensible();
        //EN REALIDAD ESTE NO ME SIRVE, PERO LO DEJO PARA QUE NO FALLEN LOS OTROS ARCHIVOS*********

		HashingExtensible(std::string directorioSalida, std::string fileName);

		virtual ~HashingExtensible();

		virtual int agregar(RegistroVariable& reg);
		//Guarda el elemento en el hash, en caso de que ya existe el elemento devuelve YA_EXISTE

		virtual int create(std::string directorioSalida, std::string fileName);
		//NO SE PARA QUE LO PUSIERON*************************

        virtual RegistroVariable devolver(int clave);
        //Devuelve el elemento del hash, en caso de que no exista devuelve NO_EXISTE

        virtual int modificar(int clave, RegistroVariable elemN);
        //Modifica el elemento buscado, en caso de no encontrarlo devuelve NO_EXISTE

        virtual int eliminar(int clave);
        //Elimina el elemento

    private:

        virtual int funcion_dispersion(int clave);

};

#endif /* HASHINGEXTENSIBLE_H */
