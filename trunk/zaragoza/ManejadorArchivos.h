/*
 * ManejadorArchivos.h
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#ifndef MANEJADORARCHIVOS_H_
#define MANEJADORARCHIVOS_H_

#include<stdlib.h>
#include<stdio.h>
#include<fstream>

class ManejadorArchivos {

	public:
		const static unsigned int ARCHIVO_NO_EXISTE= 0;
		const static unsigned int ARCHIVO_EXISTE= 1;
		const static unsigned int FRACASO= 0;
		const static unsigned int EXITO= 1;
		const static unsigned int ARGUMENTOS_INSUFICIENTES= 2;

		virtual int crear_archivo(char* nombreArchivo);
		virtual bool archivo_existe(char* nombreArchivo)const throw();
		virtual int eliminar_archivo(char* nombreArchivo)const throw();
		virtual int manejar(int argc,char** args)= 0;
		virtual int mostrar_archivo(char* nombreArchivo)= 0;

		virtual ~ManejadorArchivos(){};

};

#endif /* MANEJADORARCHIVOS_H_ */
