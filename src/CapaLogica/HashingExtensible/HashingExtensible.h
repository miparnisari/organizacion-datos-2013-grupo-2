#ifndef HASHINGEXTENSIBLE_H
#define HASHINGEXTENSIBLE_H

#include "Tabla.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../Constantes.h"
#include "../../CapaFisica/Bloque.h"
#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/Clave.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"


class HashingExtensible
{
    /*Atributos..... No se necesitan guardar en el archivo pero son utiles en RAM*/
    private:

        std::string fileName;
        std::string fileNameTabla;
        int cant_bloques;
        ManejadorBloques manejador_bloques;
      //  Tabla tabla;


	public:

        HashingExtensible();
        //EN REALIDAD ESTE NO ME SIRVE, PERO LO DEJO PARA QUE NO FALLEN LOS OTROS ARCHIVOS*********

		HashingExtensible(std::string directorioSalida, std::string fileName);

		virtual ~HashingExtensible();

		virtual int agregar(RegistroClave reg);
		//Guarda el elemento en el hash, en caso de que ya existe el elemento devuelve YA_EXISTE

		virtual int create(std::string directorioSalida, std::string fileName);
		//NO SE PARA QUE LO PUSIERON*************************

        virtual int devolver(ClaveX clave, RegistroClave *reg);
        //Devuelve el elemento del hash, en caso de que no exista devuelve NO_EXISTE

        virtual int modificar(RegistroClave elemN);
        //Modifica el elemento buscado, en caso de no encontrarlo devuelve NO_EXISTE

        virtual int eliminar(ClaveX clave);
        //Elimina el elemento

    private:

        virtual int funcion_dispersion(int clave);
        //Devuelve el numero de bloque donde se debe guardar esa clave

        virtual int crear_bloque(int tam, Bloque *bloqueNuevo);
        //Crea un nuevo bloque del Hashing

        virtual int guardar_tabla(Tabla *tabla);
        //Guarda la tabla en el archivo del Hashing

        virtual char* obtener_tabla();
        //virtual int obtener_tabla(Tabla *tabla);
        //Nos devuelve la tabla que esta guardada en el archivo del Hashing

        virtual int obtener_bloque(ClaveX clave, Bloque *bloque);
        //Obtenemos el bloque que corresponde a la clave pasada por parametro

        virtual int obtener_posicion_reg_bloque(ClaveX clave, Bloque bloque);
        //Obtenemos en que posicion se encuentra el registro con la clave que buscamos
        //dentro de un bloque

        virtual int obtener_posicion_bloque(ClaveX elemN);
        //Dependiendo del tipo de clave que guarde aplica una funcion para sacar un numero
        //al cual le aplica la funcion de hashing para sacar la posicion del bloque para esa clave

        virtual int agregar_registros_bloques(Bloque bloque, RegistroClave reg);
        //Lo que basicamente es tomar todos los registros del bloque que desbordo junto al nuevo
        //registro y los guarda en el hashing como cualquier registro
};

#endif /* HASHINGEXTENSIBLE_H */
