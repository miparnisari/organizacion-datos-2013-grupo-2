/*
 * ManejadorArchivos.h
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#ifndef MANEJADORARCHIVOS_H_
#define MANEJADORARCHIVOS_H_

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "../Constantes.h"

class ManejadorArchivos {

	public:
		virtual int crear_archivo(std::string nombreArchivo);
		virtual bool archivo_existe(std::string nombreArchivo)const throw();
		virtual int eliminar_archivo(std::string nombreArchivo)const throw();
		virtual int mostrar_archivo(std::string nombreArchivo)= 0;
		virtual int manejar(int argc,char** args) = 0;
		
		virtual ~ManejadorArchivos(){};

};

#endif /* MANEJADORARCHIVOS_H_ */
