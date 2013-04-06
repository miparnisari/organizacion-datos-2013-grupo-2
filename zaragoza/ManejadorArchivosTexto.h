/*
 * ManejadorArchivosTexto.h
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#ifndef MANEJADORARCHIVOSTEXTO_H_
#define MANEJADORARCHIVOSTEXTO_H_

#include"ManejadorArchivos.h"
#include<iostream>

using namespace std;

class ManejadorArchivosTexto:public ManejadorArchivos {

	protected:


	public:
		static const int LINEAS_NO_HALLADAS= -1;

		int agregar_linea(char* nombreArchivo,char* linea)const throw();
		int mostrar_archivo(char* nombreArchivo);
		int hallar_lineas(char* nombreArchivo, char* linea)const throw();

		int manejar(int argc,char** args);

};

#endif /* MANEJADORARCHIVOSTEXTO_H_ */
