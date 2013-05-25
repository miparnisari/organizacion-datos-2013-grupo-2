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
#include "../../Utilitarios/Utilitarios.h"


class HashingExtensible
{
    /*Atributos..... No se necesitan guardar en el archivo pero son utiles en RAM*/
    private:
        std::string fileName;
        std::string fileNameTabla;
        int cant_bloques;
        ManejadorBloques* manejador_bloques;
        Tabla* tabla;

        public:

        HashingExtensible();

		virtual ~HashingExtensible();

		virtual int crear_archivo(std::string nombreArchivo);
		virtual int eliminar_archivo();

		virtual int abrir_archivo(std::string nombreArchivo);
		virtual int cerrar_archivo();



		virtual int agregar(RegistroClave & reg);
        virtual int devolver(ClaveX & clave, RegistroClave *reg);
        virtual int modificar(RegistroClave & elemN);
        virtual int eliminar(ClaveX & clave);

    private:

        virtual int _funcion_dispersion(int clave);
        //Devuelve el numero de bloque donde se debe guardar esa clave

        virtual int _crear_bloque(int tam, Bloque** bloqueNuevo);
        //Crea un nuevo bloque del Hashing

        virtual int _obtener_bloque(ClaveX & clave, Bloque** bloque);
        //Obtenemos el bloque que corresponde a la clave pasada por parametro

        virtual int _obtener_posicion_reg_bloque(ClaveX & clave,Bloque & bloque);
        //Obtenemos en que posicion se encuentra el registro con la clave que buscamos
        //dentro de un bloque

        virtual int _obtener_posicion_tabla(ClaveX & elemN);
        //Dependiendo del tipo de clave que guarde aplica una funcion para sacar un numero
        //al cual le aplica la funcion de hashing para sacar la posicion de la tabla para esa clave

        virtual int _agregar_registros_bloques(Bloque & bloque, RegistroClave & reg);
        //Tomar todos los registros del bloque que desbordo junto al nuevo
        //registro y los guarda en el hashing como cualquier registro
};

#endif /* HASHINGEXTENSIBLE_H */
