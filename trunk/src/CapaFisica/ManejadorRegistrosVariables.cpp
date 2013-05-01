/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;


void ManejadorRegistrosVariables::_pegar_registro(RegistroVariable* registro,long offset){



}


long ManejadorRegistrosVariables::_buscar_registro_libre(unsigned short espacioNecesario){

	if(header.cantidadRegistrosLibres== 0)
		return RES_ERROR;

	unsigned short espacioRegistroLibre= 0;
	fstream archivo(nombreArchivo.c_str());
	long offsetRegistroLibre= header.offsetPrimerRegistroLibre;
	const unsigned short ESPACIO_REQUERIDO= espacioNecesario + sizeof(HeaderRegistroLibre) -
			sizeof( unsigned short );
	/*dado que un registro libre al ser reaprovechado se partira en dos (una parte se
	 * ocupara y otra permanecera libre), se necesita  suficiente espacio para guardar
	 * el nuevo registro y un HeaderRegistroLibre contiguos .*/

	while(espacioRegistroLibre< ESPACIO_REQUERIDO && offsetRegistroLibre!= -1){

		archivo.seekg(offsetRegistroLibre , ios::beg);
		HeaderRegistroLibre hrl;
		archivo.read( (char*)&hrl , sizeof(hrl) );
		espacioRegistroLibre= hrl.espacioLibre;

		if(espacioRegistroLibre < ESPACIO_REQUERIDO)
			offsetRegistroLibre= hrl.offsetProximoRegistroLibre;

	}

	_cerrar_archivo(&archivo);
	return offsetRegistroLibre;

}


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
	char* bufferRegistro= new char[tamanioRegistro + sizeof(tamanioRegistro)]();
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

	HeaderRegistroLibre hre;
	memset(&hre,0,sizeof(hre));
	hre.espacioLibre= TAMANIO_REGISTRO_ELIMINAR;
	hre.marcaBorrado= MARCA_BORRADO;
	hre.offsetProximoRegistroLibre= header.offsetPrimerRegistroLibre;

	stringstream streamEliminacion;
	streamEliminacion.write( (char*)&hre , sizeof(hre) );
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


long ManejadorRegistrosVariables::_append_registro(RegistroVariable* registro){

	fstream archivo(nombreArchivo.c_str());
	archivo.seekp(0,ios::end);
	long offset= archivo.tellp();
	const int TAMANIO_EMPAQUETADO= registro->get_tamanio_empaquetado();
	char* buffer= new char[TAMANIO_EMPAQUETADO]();

	registro->empaquetar(buffer);
	archivo.write(buffer,TAMANIO_EMPAQUETADO);
	header.cantidadRegistros++;
	header.tamanioArchivo+= TAMANIO_EMPAQUETADO;

	delete[] buffer;
	_cerrar_archivo(&archivo);
	_guardar_header();
	return offset;

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


long ManejadorRegistrosVariables::agregar_registro(RegistroVariable* registro){

	if(!archivo_existe(nombreArchivo))
		return RES_ERROR;
	if(registro== NULL)
		return RES_ERROR;

	_leer_header();
	const unsigned short TAMANIO_EMPAQUETAMIENTO= registro->get_tamanio_empaquetado();
	const long OFFSET_REGISTRO_LIBRE= _buscar_registro_libre(TAMANIO_EMPAQUETAMIENTO);

/*
	if(header.offsetPrimerRegistroLibre== -1){
		this->_append_registro(registro);
		return RES_OK;
	}
	TODO si no funca eliminar comentarios*/

	if(OFFSET_REGISTRO_LIBRE== RES_ERROR){
		return this->_append_registro(registro);
	}


	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(OFFSET_REGISTRO_LIBRE , ios::beg);
	HeaderRegistroLibre hrlInicial;
	archivo.read( (char*)&hrlInicial , sizeof(hrlInicial) );
	HeaderRegistroLibre hrlFinal= hrlInicial;
	/*obtengo los datos del registro libre a usar .*/

	unsigned short tamanioEscribir= hrlInicial.espacioLibre + sizeof(unsigned short);
	/*calculo el tamanio del buffer a escribir en el archivo*/

	hrlFinal.espacioLibre-= TAMANIO_EMPAQUETAMIENTO;
	/*el espacio del registro libre particionado es el anterior menos el tamanio del
	 * registro a escribir*/


	archivo.seekg(OFFSET_REGISTRO_LIBRE , ios::beg);
	unsigned short tamanioResto= tamanioEscribir - TAMANIO_EMPAQUETAMIENTO -
			sizeof(HeaderRegistroLibre);
	char* bufferResto= new char[tamanioResto]();
	/*obtengo el resto de datos libres*/


	char* bufferRegistro= new char[TAMANIO_EMPAQUETAMIENTO]();
	registro->empaquetar(bufferRegistro);
	/*empaqueto el registro a guardar*/


	stringstream stream;
	stream.write( (char*)&hrlFinal , sizeof(hrlFinal) );
	stream.write( bufferResto , tamanioResto );
	stream.write( bufferRegistro,TAMANIO_EMPAQUETAMIENTO );
	/*guardo en el stream registroLibre|registroNuevo */


	char* bufferEscribir= new char[tamanioEscribir]();
	stream.seekg(0,ios::beg);
	stream.read(bufferEscribir , tamanioEscribir);


	archivo.write( bufferEscribir , tamanioEscribir );



	delete[] bufferResto;
	delete[] bufferRegistro;
	delete[] bufferEscribir;


	_cerrar_archivo(&archivo);
	header.cantidadRegistros++;
	_guardar_header();

	long offsetRetornar= OFFSET_REGISTRO_LIBRE + sizeof(HeaderRegistroLibre)
			+ tamanioResto;
	return offsetRetornar;

}





ManejadorRegistrosVariables::ManejadorRegistrosVariables():ManejadorArchivos(){

	_desvincular();
	_resetear_header();

}
