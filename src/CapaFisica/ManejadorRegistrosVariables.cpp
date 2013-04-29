/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;


long ManejadorRegistrosVariables::_get_offset_registro(unsigned short numeroRegistro){

	_leer_header();
	if(numeroRegistro >= header.cantidadRegistros)
		return RES_ERROR;

	long contadorOffset= OFFSET_PRIMER_REGISTRO;
	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(OFFSET_PRIMER_REGISTRO,ios::beg);

	for(int i=0;i<numeroRegistro;i++){

		unsigned short tamanioRegistro;
		archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
		archivo.seekg( tamanioRegistro , ios::cur );

		contadorOffset+= tamanioRegistro + sizeof(tamanioRegistro);

	}


	_cerrar_archivo(&archivo);
	return contadorOffset;

}


long ManejadorRegistrosVariables::get_registro(RegistroVariable* registro,
		unsigned short numeroRegistro){

	if(_registro_fue_eliminado(numeroRegistro))
		return RES_ERROR;//TODO verificar que funque!!!

	/*_leer_header();
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

		contadorOffset+= tamanioRegistro + sizeof(tamanioRegistro);
		contadorRegistros++;

	}*/

	long contadorOffset= _get_offset_registro(numeroRegistro);
	fstream archivo(nombreArchivo.c_str());
	archivo.seekg( contadorOffset,ios::beg );


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
	_cerrar_archivo(&archivo);

	return contadorOffset;

}


bool ManejadorRegistrosVariables::_registro_fue_eliminado(unsigned short numeroRegistro){

	long offset= _get_offset_registro(numeroRegistro);

	if(offset== RES_ERROR)
		return true;

	if(header.cantidadRegistrosLibres== 0)
		return false;

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offset, ios::beg);
	unsigned short tamanioRegistro;
	archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );

	char* bufferRegistro= new char[tamanioRegistro]();
	archivo.read( bufferRegistro , tamanioRegistro );
	if( bufferRegistro[0]!= MARCA_BORRADO ){
		delete[] bufferRegistro;
		this->_cerrar_archivo(&archivo);
		return false;
	}

	delete[] bufferRegistro;
	_cerrar_archivo(&archivo);
	return true;

}


long ManejadorRegistrosVariables::eliminar_registro(unsigned short numeroRegistro){

	if(this->_registro_fue_eliminado(numeroRegistro))
		return RES_ERROR;
	/*verifico que el registro a buscar no haya sido eliminado*/

	RegistroVariable registroEliminar;
	long offset=  this->get_registro(&registroEliminar , numeroRegistro);

	const unsigned short ESPACIO_LIBERADO= registroEliminar.get_tamanio_empaquetado();
	const unsigned short TAMANIO_REGISTRO_ELIMINAR= registroEliminar.get_tamanio();
	char* bufferEliminacion= new char[ESPACIO_LIBERADO]();
	stringstream streamEliminacion;

	streamEliminacion.write( (char*)&TAMANIO_REGISTRO_ELIMINAR , sizeof(TAMANIO_REGISTRO_ELIMINAR) );
	streamEliminacion.write( (char*)&MARCA_BORRADO , sizeof(MARCA_BORRADO) );
	streamEliminacion.write( (char*)&header.offsetPrimerRegistroLibre ,
			sizeof(header.offsetPrimerRegistroLibre) );
	header.offsetPrimerRegistroLibre= offset;
	header.cantidadRegistrosLibres++;
	/*escribo en el stream:  tamanioRegistroLibre| * | offsetSigRegLibre.
	 * Apilo los registrosLibres. */

	streamEliminacion.seekg(0,ios::beg);
	streamEliminacion.read( bufferEliminacion,ESPACIO_LIBERADO );
	/*escribo en el bufferEliminacion aquello que guarde en el stream*/

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offset , ios::beg);
	archivo.write(bufferEliminacion,ESPACIO_LIBERADO);

	delete[] bufferEliminacion;
	_cerrar_archivo(&archivo);
	_guardar_header();
	return offset;

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
	header.offsetPrimerRegistroLibre= -1;
	header.tamanioArchivo= sizeof(header);
	header.cantidadRegistrosLibres= 0;

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


int ManejadorRegistrosVariables::get_cantidad_registros_ocupados(){

	return (header.cantidadRegistros - header.cantidadRegistrosLibres);

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
	if(header.offsetPrimerRegistroLibre== -1){
		this->_append_registro(registro);
		return RES_OK;
	}





	return RES_OK;

}





ManejadorRegistrosVariables::ManejadorRegistrosVariables():ManejadorArchivos(){

	_desvincular();
	_resetear_header();

}
