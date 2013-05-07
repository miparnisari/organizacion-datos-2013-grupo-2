/*
 * ManejadorArchivosTexto.h
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#ifndef MANEJADORARCHIVOSTEXTO_H_
#define MANEJADORARCHIVOSTEXTO_H_

#include "ManejadorArchivos.h"
#include <iostream>

class ManejadorArchivosTexto:public ManejadorArchivos {

	protected:


	public:
		static const int LINEAS_NO_HALLADAS = -1;

		int agregar_linea(std::string nombreArchivo, std::string linea)const throw();
		int mostrar_archivo(std::string nombreArchivo);
		int hallar_lineas(std::string nombreArchivo, std::string linea)const throw();

};

#endif /* MANEJADORARCHIVOSTEXTO_H_ */
