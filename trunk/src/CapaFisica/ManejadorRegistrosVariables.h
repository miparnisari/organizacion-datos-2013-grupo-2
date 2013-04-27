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
		int agregar_registro(std::string nombreArchivo,std::string dato)const throw();
		int registrar_archivo(std::string nombreArchivoDestino,std::string nombreArchivoFuente)const throw();
		int mostrar_archivo(std::string nombreArchivo);
		int get_registro(std::string nombreArchivo,RegistroVariable* registro,unsigned int numeroRegistro);
		int mostrar_registro(std::string nombreArchivo,unsigned int numeroRegistro);
		int buscar_registro_por_texto(std::string nombreArchivo,std::string texto)throw();
		int mostrar_offsets(std::string nombreArchivo)throw();

		int mostrar_caracteristicas_registro(std::string nombreArchivo,unsigned int numeroRegistro);
		int mostrar_caracteristicas_registros(std::string nombreArchivo);

		int manejar(int argc,char** args);

};

#endif /* MANEJADORREGISTROSVARIABLES_H_ */
