/*
 * ManejadorArchivos.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#include"ManejadorArchivos.h"
#include<iostream>
#include<fstream>

using namespace std;

int ManejadorArchivos::crear_archivo(char* nombreArchivo){

	string sna= nombreArchivo;

	ofstream archivoNuevo(sna.c_str());
	if(!archivoNuevo.is_open())
		return FRACASO;

	archivoNuevo.close();

	return EXITO;


}/*crea un archivo sin extension .txt en un archivo de texto*/


bool ManejadorArchivos::archivo_existe(char* nombreArchivo)const throw(){

	ifstream archivo;
	archivo.open(nombreArchivo,ios::in);
	bool resultado= archivo.good();

	if(resultado){
		archivo.seekg(0,ios::end);
		archivo.close();
	}

	return resultado;

}/*chequea si un archivo existe*/


int ManejadorArchivos::eliminar_archivo(char* nombreArchivo)const throw(){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	string comando= "rm ";
	comando+= nombreArchivo;
	system(comando.c_str());

	return EXITO;

}/*elimina un archivo dada una ruta*/
