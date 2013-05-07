/*
 * ManejadorArchivosTexto.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#include "ManejadorArchivosTexto.h"


int ManejadorArchivosTexto::agregar_linea(std::string nombreArchivo, std::string linea)const throw(){

	std::ifstream archivoLectura;
	archivoLectura.open(nombreArchivo.c_str(),std::ios::in);
	if(!archivoLectura.good())
		return RES_FILE_DOESNT_EXIST;

	archivoLectura.seekg(0,std::ios::end);
	archivoLectura.close();

	std::ofstream archivoEscritura(nombreArchivo.c_str(),std::ios::app);
	archivoEscritura<<linea;
	archivoEscritura<<'\n';

	archivoEscritura.close();
	return RES_OK;

}/*agrega linea al final de un archivo existente*/


int ManejadorArchivosTexto::mostrar_archivo(std::string nombreArchivo){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	std::ifstream archivo(nombreArchivo.c_str(),std::ios::in);
	const unsigned short TAMANIO_BUFFER= 1024;
	char* buffer= new char[TAMANIO_BUFFER];

	while(archivo.good()){

		archivo.getline(buffer,TAMANIO_BUFFER);
		if(archivo.good()){
			std::cout<<buffer<<std::endl;
		}

	}

	delete[] buffer;
	return RES_OK;

}/*muestra lineas de un archivo por pantalla*/


int ManejadorArchivosTexto::hallar_lineas(std::string nombreArchivo,std::string linea)const throw(){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	const unsigned short TAMANIO_BUFFER_CHAR= 1024;
	char* bufferChar= new char[TAMANIO_BUFFER_CHAR];
	std::ifstream archivo(nombreArchivo.c_str(),std::ios::in);
	int posicionLinea= LINEAS_NO_HALLADAS;
	std::string lineaString= linea;

	while(archivo.good()){

		archivo.getline(bufferChar,TAMANIO_BUFFER_CHAR);

		if(archivo.good()){
			std::string bufferString= bufferChar;
			posicionLinea= bufferString.find(lineaString);
			if(posicionLinea!= LINEAS_NO_HALLADAS)
				std::cout<<bufferString<<std::endl;
		}

	}

	archivo.seekg(0,std::ios::end);
	archivo.close();

	delete[] bufferChar;
	return posicionLinea;

}/*halla lineas del archivo de texto donde se encuentra una linea especifica*/

