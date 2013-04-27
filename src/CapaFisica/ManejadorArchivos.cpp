/*
 * ManejadorArchivos.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#include"ManejadorArchivos.h"
#include<iostream>
#include<fstream>

int ManejadorArchivos::crear_archivo(std::string nombreArchivo){

	std::ofstream archivoNuevo(nombreArchivo.c_str());
	if (!archivoNuevo.is_open())
		return RES_ERROR;

	archivoNuevo.close();

	return RES_OK;


}/*crea un archivo sin extension .txt en un archivo de texto*/


bool ManejadorArchivos::archivo_existe(std::string nombreArchivo)const throw(){

	std::ifstream archivo;
	archivo.open(nombreArchivo.c_str(),std::ios::in);
	bool resultado= archivo.good();

	if(resultado){
		archivo.seekg(0,std::ios::end);
		archivo.close();
	}

	return resultado;

}/*chequea si un archivo existe*/


int ManejadorArchivos::eliminar_archivo(std::string nombreArchivo)const throw(){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_EXISTS;

	std::string comando= "rm ";
	comando += nombreArchivo;
	system(comando.c_str());

	return RES_OK;

}/*elimina un archivo dada una ruta*/
