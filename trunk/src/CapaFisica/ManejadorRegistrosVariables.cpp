/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;

ManejadorRegistrosVariables::ManejadorRegistrosVariables():ManejadorArchivos(){

	_desvincular();
	_resetear_header();

}


void ManejadorRegistrosVariables::_pegar_registro(RegistroVariable* registro,long offset){



}


long ManejadorRegistrosVariables::_buscar_registro_libre(unsigned short espacioNecesario,
		long& offsetLibreAnterior , long& offsetLibreSiguiente){

	if(header.cantidadRegistrosLibres== 0)
		return RES_ERROR;

	offsetLibreAnterior= offsetLibreSiguiente= FIN_PILA_REGISTROS_LIBRES;

	unsigned short espacioRegistroLibre= 0;
	fstream archivo(nombreArchivo.c_str());
	long offsetRegistroLibre= header.offsetPrimerRegistroLibre;
	const unsigned short ESPACIO_REQUERIDO= espacioNecesario + sizeof(HeaderRegistroLibre) -
			sizeof( unsigned short );
	/*dado que un registro libre al ser reaprovechado se partira en dos (una parte se
	 * ocupara y otra permanecera libre), se necesita  suficiente espacio para guardar
	 * el nuevo registro y un HeaderRegistroLibre contiguos .*/

	while(espacioRegistroLibre< ESPACIO_REQUERIDO && offsetRegistroLibre!= FIN_PILA_REGISTROS_LIBRES){

		archivo.seekg(offsetRegistroLibre , ios::beg);
		HeaderRegistroLibre hrl;
		archivo.read( (char*)&hrl , sizeof(hrl) );
		offsetLibreSiguiente= hrl.offsetProximoRegistroLibre;
		espacioRegistroLibre= hrl.espacioLibre;

		if(espacioRegistroLibre < ESPACIO_REQUERIDO){
			offsetLibreAnterior= offsetRegistroLibre;
			offsetRegistroLibre= hrl.offsetProximoRegistroLibre;
		}

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

/*
long ManejadorRegistrosVariables::get_registro(RegistroVariable* registro,
		unsigned short numeroRegistro){

	if(_registro_fue_eliminado(numeroRegistro))
		return RES_ERROR;//TODO verificar que funque!!!


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
*/

int ManejadorRegistrosVariables::get_registro_por_offset(RegistroVariable* registro ,
		unsigned long offset){

	if(offset < OFFSET_PRIMER_REGISTRO || offset > header.tamanioArchivo)
		return RES_ERROR;
	if( !archivo_existe(nombreArchivo) || registro== NULL )
		return RES_ERROR;

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offset,ios::beg);
	unsigned short tamanioRegistro;
	archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
	unsigned short const TAMANIO_EMPAQUETADO= tamanioRegistro
			+ sizeof(tamanioRegistro);
	char* bufferRegistro= new char[TAMANIO_EMPAQUETADO]();
	archivo.read( bufferRegistro , tamanioRegistro );
	/*se lee el tamanio del registro y el registro .*/

	stringstream stream;
	stream.write( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
	stream.write( bufferRegistro , tamanioRegistro );
	stream.seekg(0,ios::beg);
	stream.read( bufferRegistro , TAMANIO_EMPAQUETADO );
	/*se carga el registro empaquetado en un buffer*/

	registro->desempaquetar( bufferRegistro );

	delete[] bufferRegistro;
	_cerrar_archivo(&archivo);
	return RES_OK;

}


long ManejadorRegistrosVariables::get_registro_ocupado(RegistroVariable* registro ,
		unsigned short numeroRegistro){

	if( !archivo_existe(nombreArchivo) )
		return RES_ERROR;
	if( numeroRegistro >= this->get_cantidad_registros_ocupados() )
		return RES_ERROR;

	fstream archivo( nombreArchivo.c_str() );
	archivo.seekg(OFFSET_PRIMER_REGISTRO , ios::beg);
	/*me paro en el primer registro*/
	unsigned short contadorRegistros= 0;
	long contadorOffset= OFFSET_PRIMER_REGISTRO;
	char* bufferRegistro;
	unsigned short tamanioRegistro , tamanioEmpaquetado;


	while( contadorRegistros <= numeroRegistro ){

		archivo.read( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
		tamanioEmpaquetado= tamanioRegistro + sizeof(tamanioRegistro);
		bufferRegistro= new char[tamanioEmpaquetado]();
		archivo.read( bufferRegistro , tamanioRegistro );

		if( bufferRegistro[0]!= MARCA_BORRADO ){
			contadorRegistros++ ;
		}/*si el registro levantado no fue eliminado*/

		if(contadorRegistros <= numeroRegistro){
			delete[] bufferRegistro;
			contadorOffset+= tamanioEmpaquetado;
		}

	}/*mientras no se haya llegado al numero de registro*/

	stringstream stream;
	stream.write( (char*)&tamanioRegistro , sizeof(tamanioRegistro) );
	stream.write( bufferRegistro , tamanioRegistro );
	stream.seekg( 0,ios::beg );
	stream.read( bufferRegistro , tamanioEmpaquetado );
	registro->desempaquetar( bufferRegistro );

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

/*
long ManejadorRegistrosVariables::eliminar_registro(unsigned short numeroRegistro){

	if(this->_registro_fue_eliminado(numeroRegistro))
		return RES_ERROR;
	//verifico que el registro a buscar no haya sido eliminado

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
	//escribo en el stream:  tamanioRegistroLibre| * | offsetSigRegLibre.
	 // Apilo los registrosLibres.

	streamEliminacion.seekg(0,ios::beg);
	streamEliminacion.read( bufferEliminacion,ESPACIO_LIBERADO );
	//escribo en el bufferEliminacion aquello que guarde en el stream

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offset , ios::beg);
	archivo.write(bufferEliminacion,ESPACIO_LIBERADO);

	delete[] bufferEliminacion;
	_cerrar_archivo(&archivo);
	_guardar_header();
	return offset;

}
*/

long ManejadorRegistrosVariables::eliminar_registro_ocupado(unsigned short numeroRegistro){

	if(!archivo_existe(nombreArchivo))
		return RES_ERROR;

	RegistroVariable registroEliminar;
	long offsetRegistroEliminar= this->get_registro_ocupado(&registroEliminar,
			numeroRegistro);

	if(offsetRegistroEliminar == RES_ERROR )
		return RES_ERROR;

	_leer_header();
	long offsetProximoRegistroLibre= header.offsetPrimerRegistroLibre;
	header.offsetPrimerRegistroLibre = offsetRegistroEliminar;
	/*coloco al registro a eliminar como cabeza de la pila de registros libres*/

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offsetRegistroEliminar , ios::beg);
	unsigned short tamanioRegistroEliminar;
	archivo.read( (char*)&tamanioRegistroEliminar , sizeof(tamanioRegistroEliminar) );
	_cerrar_archivo(&archivo);
	/*leo el tamanio del registro a borrar*/

	HeaderRegistroLibre hrl;
	hrl.espacioLibre= tamanioRegistroEliminar;
	hrl.marcaBorrado= MARCA_BORRADO;
	hrl.offsetProximoRegistroLibre= offsetProximoRegistroLibre;
	/*creo el header que ocupara el espacio del registro eliminado*/

	fstream archivoEscribir(nombreArchivo.c_str());
	if (archivoEscribir.good())
	{
		archivoEscribir.seekp( offsetRegistroEliminar , ios::beg );
		archivoEscribir.write( (char*)&hrl , sizeof(hrl) );
		_cerrar_archivo(&archivoEscribir);
		/*se escribe la marca de borrado */
	}

	header.cantidadRegistrosLibres++;
	_guardar_header();
	return offsetRegistroEliminar;

}


long ManejadorRegistrosVariables::_append_registro(RegistroVariable* registro){

	fstream archivo(nombreArchivo.c_str());
	archivo.seekp(0,ios::end);
	long offset= archivo.tellp();
	const unsigned short TAMANIO_EMPAQUETADO= registro->get_tamanio_empaquetado();
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

	//archivo->seekg(0,ios::end);
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
	if (archivo.good()) {
		archivo.write( (char*)&header , sizeof(header) );

		_cerrar_archivo(&archivo);
	}

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
	long offsetLibreAnterior,offsetLibreSiguiente;
	const long OFFSET_REGISTRO_LIBRE= _buscar_registro_libre(TAMANIO_EMPAQUETAMIENTO,
			offsetLibreAnterior,offsetLibreSiguiente);

/*
	if(header.offsetPrimerRegistroLibre== -1){
		this->_append_registro(registro);
		return RES_OK;
	}
	TODO si no anda eliminar comentarios*/

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

	if(offsetLibreAnterior!= FIN_PILA_REGISTROS_LIBRES){

		fstream archivoEscribir(nombreArchivo.c_str());
		archivoEscribir.seekg(offsetLibreAnterior , ios::beg);
		HeaderRegistroLibre hrl;
		archivoEscribir.read( (char*)&hrl , sizeof(hrl) );
		hrl.offsetProximoRegistroLibre= offsetLibreSiguiente;
		archivoEscribir.seekp( offsetLibreAnterior , ios::beg );
		archivoEscribir.write( (char*)&hrl , sizeof(hrl) );
		_cerrar_archivo( &archivoEscribir );

	}/* ... si el registro recuperado es diferente al apuntado por el header del archivo*/
	else{

		header.offsetPrimerRegistroLibre = offsetLibreSiguiente;
		_guardar_header();

	}/*... si el registro libre recuperado fue aquel apuntado por el header del archivo*/


	return offsetRetornar;

}


int ManejadorRegistrosVariables::eliminar_registro_por_offset(unsigned long offset){

	if(!archivo_existe(nombreArchivo))
		return RES_ERROR;
	_leer_header();

	if(offset < OFFSET_PRIMER_REGISTRO || offset>= header.tamanioArchivo)
		return RES_ERROR;

	long offsetProximoRegistroLibre= header.offsetPrimerRegistroLibre;
	header.offsetPrimerRegistroLibre = offset;
	HeaderRegistroLibre hrl;
	hrl.offsetProximoRegistroLibre= offsetProximoRegistroLibre;
	hrl.marcaBorrado= MARCA_BORRADO;

	fstream archivo(nombreArchivo.c_str());
	archivo.seekg(offset,ios::beg);
	unsigned short tamanioRegistroEliminar;
	archivo.read( (char*)&tamanioRegistroEliminar , sizeof(tamanioRegistroEliminar) );
	_cerrar_archivo(&archivo);

	hrl.espacioLibre= tamanioRegistroEliminar;
	fstream archivoEscribir(nombreArchivo.c_str());
	archivoEscribir.seekp(offset , ios::beg);
	archivoEscribir.write( (char*)&hrl , sizeof(hrl) );
	_cerrar_archivo(&archivoEscribir);

	header.cantidadRegistrosLibres++;
	_guardar_header();
	return RES_OK;

}
