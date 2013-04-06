/*
 * ManejadorBloques.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: martin
 */

#include "ManejadorBloques.h"

int ManejadorBloques::contar_bloques(char* nombreArchivo)throw(){

	/*if(!archivo_existe(nombreArchivo))
		return ERROR;

	ifstream archivo(nombreArchivo);
	const unsigned int TAMANIO_BLOQUE= Bloque::TAMANIO_BLOQUE;
	char buffer[TAMANIO_BLOQUE];
	int cuentaBloques= 0;

	while(archivo.good()){

		archivo.read(buffer,TAMANIO_BLOQUE);
		if(archivo.good())
			cuentaBloques++;

	}

	archivo.close();
	return cuentaBloques;*/

	if(!archivo_existe(nombreArchivo))
		return ERROR;

	fstream archivo(nombreArchivo);
	archivo.seekg(0,ios::end);
	unsigned int tamanioArchivo= archivo.tellg();
	unsigned int cantidadBloques= (unsigned int)(tamanioArchivo / tamanioBloque);

	archivo.close();
	return cantidadBloques;

}/*cuenta la cantidad de bloques en un archivo.*/


bool ManejadorBloques::esta_vacio(char* nombreArchivo)throw(){

	return (this->contar_bloques(nombreArchivo)== 0);

}/*chequea si hay algun bloque en el archivo*/

/*
int ManejadorBloques::hallar_bloque_libre(ifstream& archivo,Bloque* bloque,unsigned int espacioNecesario)throw(){

	const unsigned int TAMANIO_BUFFER= Bloque::TAMANIO_BLOQUE;
	char buffer[TAMANIO_BUFFER];
	int cuentaBloques= 0;
	bool bloqueLibreHallado= false;
	Bloque b;

	while(archivo.good() && !bloqueLibreHallado){

		archivo.read(buffer,TAMANIO_BUFFER);

		if(archivo.good()){
			b.cargar(buffer);
			if(b.get_espacio_libre() >)
		}

	}

}*/


int ManejadorBloques::agregar_registro(char* nombreArchivo,char* dato,unsigned int tamanioDato)throw(){

	if(!archivo_existe(nombreArchivo))
		return ERROR;


	fstream archivo(nombreArchivo , ios::in |ios::out);
	const unsigned int TAMANIO_BLOQUE= tamanioBloque;
	char buffer[TAMANIO_BLOQUE];
	int cuentaBloques= 0;
	int bloqueLibreHallado= 0;
	Bloque bloque(tamanioBloque);

	if(esta_vacio(nombreArchivo)){
		bloque.agregar_registro(dato,tamanioDato);
		bloque.guardar(archivo);
		archivo.close();
		return 0;
	}


	while( archivo.good() && bloqueLibreHallado!= Bloque::EXITO ){

		archivo.read(buffer,TAMANIO_BLOQUE);

		if(archivo.good()){
			cuentaBloques++;
			bloque.cargar(buffer);
			bloqueLibreHallado= bloque.agregar_registro(dato,tamanioDato);
		}

	}

	if(bloqueLibreHallado== Bloque::EXITO){

		unsigned int offset= TAMANIO_BLOQUE*(cuentaBloques-1);
		archivo.seekp(offset,archivo.beg);
		bloque.guardar(archivo);
		archivo.seekg(0,archivo.end);
		return (cuentaBloques-1);

	}

	bloque.limpiar();
	bloque.agregar_registro(dato,tamanioDato);
	archivo.seekp(0,archivo.end);
	bloque.guardar(archivo);

	return cuentaBloques;

}/*agrega un registro en un bloque con suficiente espacio libre*/


int ManejadorBloques::agregar_archivo(char* nombreArchivoDestino,char* nombreArchivoFuente)throw(){

	if( !archivo_existe(nombreArchivoDestino) || !archivo_existe(nombreArchivoFuente) )
		return ERROR;

	ifstream archivoFuente(nombreArchivoFuente);
	unsigned int tamanioArchivoFuente;

	archivoFuente.seekg(0,ios::end);
	tamanioArchivoFuente= archivoFuente.tellg();
	if(tamanioArchivoFuente >= RegistroVariable::TAMANIO_MAXIMO)
		return ERROR;


	archivoFuente.seekg(0,ios::beg);

	char buffer[tamanioArchivoFuente];
	archivoFuente.read(buffer,tamanioArchivoFuente);
	RegistroVariable rv;


	if(rv.agregar_datos(buffer,tamanioArchivoFuente) == rv.EXITO){
		this->agregar_registro(nombreArchivoDestino ,rv.get_buffer() , rv.get_tamanio() );
		return EXITO;
	}

	return FRACASO;


}/*agrega el contenido de un archivoFuente a un archivoDestino como un solo registro variable*/


int ManejadorBloques::listar_archivo_completo(char* nombreArchivo)throw(){

	if(!archivo_existe(nombreArchivo))
		return ERROR;

	char buffer[tamanioBloque];
	unsigned int cuentaBloques= 0;
	fstream archivo(nombreArchivo);

	while(archivo.good()){

		archivo.read(buffer,tamanioBloque);

		if(archivo.good()){
			Bloque b(tamanioBloque);
			b.cargar(buffer);
			cout<<"Bloque: "<<cuentaBloques<<endl;
			cout<<"-----------------------"<<endl;
			b.listar_registros();
			cuentaBloques++;
		}

	}


	return EXITO;

}/*lista los detalles de cada bloque respecto a sus registros y sus respectivos byte offsets*/


int ManejadorBloques::obtener_bloque(char* nombreArchivo,unsigned short numeroBloque,Bloque* bloque)throw(){

	int cantidadBloques= this->contar_bloques(nombreArchivo);
	if(cantidadBloques== ERROR)
		return ERROR;

	if(numeroBloque>= cantidadBloques)
		return FRACASO;

	fstream archivo(nombreArchivo);
	char buffer[tamanioBloque];
	unsigned int offset= tamanioBloque*numeroBloque;
	archivo.seekg(offset,ios::beg);

	archivo.read(buffer,tamanioBloque);
	bloque->cargar(buffer);

	return EXITO;


}/*recupera el bloque numeroBloque. */


int ManejadorBloques::listar_bloque(char* nombreArchivo,unsigned short numeroBloque)throw(){

	Bloque bloque(tamanioBloque);
	int resultado= this->obtener_bloque(nombreArchivo,numeroBloque,&bloque);

	if(resultado!= EXITO)
		return 0;

}


