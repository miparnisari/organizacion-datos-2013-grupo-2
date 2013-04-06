/*
 * ManejadorRegistrosVariables.h
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#ifndef MANEJADORREGISTROSVARIABLES_H_
#define MANEJADORREGISTROSVARIABLES_H_

#include"ManejadorArchivos.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"RegistroVariable.h"

using namespace std;

class ManejadorRegistrosVariables:public ManejadorArchivos {

	protected:


	public:
		int agregar_registro(char* nombreArchivo,char* dato)const throw();
		int registrar_archivo(char* nombreArchivoDestino,char* nombreArchivoFuente)const throw();
		int mostrar_archivo(char* nombreArchivo);
		int get_registro(char* nombreArchivo,RegistroVariable* registro,unsigned int numeroRegistro);
		int mostrar_registro(char* nombreArchivo,unsigned int numeroRegistro);
		int buscar_registro_por_texto(char* nombreArchivo,char* texto)throw();
		int mostrar_offsets(char* nombreArchivo)throw();

		int mostrar_caracteristicas_registro(char* nombreArchivo,unsigned int numeroRegistro);
		int mostrar_caracteristicas_registros(char* nombreArchivo);

		int manejar(int argc,char** args);

};

#endif /* MANEJADORREGISTROSVARIABLES_H_ */
