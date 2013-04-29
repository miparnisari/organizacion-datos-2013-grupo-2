/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;


long ManejadorRegistrosVariables::get_registro(RegistroVariable* registro,
		unsigned short numeroRegistro){

	_leer_header();
	if(numeroRegistro >= header.cantidadRegistros)
		return RES_ERROR;

	long contadorOffset= OFFSET_PRIMER_REGISTRO;
	unsigned short contadorRegistros= 0;
	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(OFFSET_PRIMER_REGISTRO,ios::beg);

	for(int i=0;i<numeroRegistro;i++){

		unsigned short tamanioRegistro;
		archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
		archivo.seekg( tamanioRegistro , ios::cur );

		contadorOffset+= tamanioRegistro;
		contadorRegistros++;

	}


	unsigned short tamanioRegistro;
	archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
	char* bufferRegistro= new char[tamanioRegistro + sizeof(tamanioRegistro)];
	archivo.read( bufferRegistro , tamanioRegistro );

	stringstream empaquetador;
	empaquetador.write( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
	empaquetador.write( bufferRegistro , tamanioRegistro );
	empaquetador.seekg(0,ios::beg);
	empaquetador.read( bufferRegistro , tamanioRegistro + sizeof(tamanioRegistro) );
	registro->desempaquetar(bufferRegistro);

	delete[] bufferRegistro;

	return contadorOffset;

}


void ManejadorRegistrosVariables::_append_registro(RegistroVariable* registro){

	fstream archivo(nombreArchivo.c_str());
	archivo.seekp(0,ios::end);
	const int TAMANIO_EMPAQUETADO= registro->get_tamanio_empaquetado();
	char* buffer= new char[TAMANIO_EMPAQUETADO];

	registro->empaquetar(buffer);
	archivo.write(buffer,TAMANIO_EMPAQUETADO);
	header.cantidadRegistros++;
	header.tamanioArchivo+= TAMANIO_EMPAQUETADO;

	delete[] buffer;
	_cerrar_archivo(&archivo);
	_guardar_header();

}


void ManejadorRegistrosVariables::_desvincular(){

	this->nombreArchivo= "~";

}


void ManejadorRegistrosVariables::_cerrar_archivo(fstream* archivo){

	archivo->seekg(0,ios::end);
	archivo->close();

}


void ManejadorRegistrosVariables::_resetear_header(){

	header.cantidadRegistros= 0;
	header.numerorPrimerRegistroLibre= -1;
	header.tamanioArchivo= sizeof(header);

}


void ManejadorRegistrosVariables::_guardar_header(){

	fstream archivo(nombreArchivo.c_str());
	archivo.write( (char*)&header , sizeof(header) );

	_cerrar_archivo(&archivo);

}


void ManejadorRegistrosVariables::_leer_header(){

	fstream archivo(nombreArchivo.c_str());

	archivo.read( (char*)&header , sizeof(header) );
	this->_cerrar_archivo(&archivo);

}


int ManejadorRegistrosVariables::abrir_archivo(string nombreArchivo){

	if(!this->archivo_existe(nombreArchivo))
		return RES_ERROR;


	this->nombreArchivo= nombreArchivo;
	_leer_header();


	return RES_OK;

}


int ManejadorRegistrosVariables::crear_archivo(string nombreArchivo){

	if(this->archivo_existe(nombreArchivo))
		return RES_ERROR;

	this->nombreArchivo= nombreArchivo;
	ofstream archivo(nombreArchivo.c_str());
	archivo.close();

	_resetear_header();
	_guardar_header();


	return RES_OK;

}


int ManejadorRegistrosVariables::get_cantidad_registros(){

	if(!this->archivo_existe(nombreArchivo))
		return RES_ERROR;

	_leer_header();

	return header.cantidadRegistros;

}

long ManejadorRegistrosVariables::get_tamanio_archivo(){

	if(!this->archivo_existe(nombreArchivo))
		return RES_ERROR;

	_leer_header();

	return header.tamanioArchivo;

}


int ManejadorRegistrosVariables::agregar_registro(RegistroVariable* registro){

	if(!archivo_existe(nombreArchivo))
		return RES_ERROR;
	if(registro== NULL)
		return RES_ERROR;

	_leer_header();
	if(header.numerorPrimerRegistroLibre== -1){
		this->_append_registro(registro);
		return RES_OK;
	}

	return RES_OK;

}


ManejadorRegistrosVariables::ManejadorRegistrosVariables():ManejadorArchivos(){

	_desvincular();
	_resetear_header();

}
