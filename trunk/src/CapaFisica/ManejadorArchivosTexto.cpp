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


int ManejadorArchivosTexto::manejar(int argc,char** args){

	if(argc== 1 || argc== 2){
		std::cout<<"argumentos insuficientes!"<<std::endl;
		return RES_INSUFFICIENT_ARGUMENTS;
	}
	/*sin argumentos suficientes, error, no se sabe que hacer*/

	enum EnumeracionArgumentos {PROGRAMA=0 , NOMBRE_ARCHIVO , COMANDO , TEXTO};
	std::string const CREAR= "-c";
	std::string const REGISTRAR= "-r";
	std::string const MOSTRAR_ARCHIVO= "-l";
	std::string const HALLAR_LINEAS= "-s";
	std::string const ELIMINAR= "-e";

	std::string comando= args[COMANDO];

	int resultado;

	if(comando == CREAR){
		resultado= this->crear_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== RES_ERROR)
			std::cout<<"error en creacion!"<<std::endl;
		return resultado;
	}

	if(comando == REGISTRAR){
		if(argc < 4){
			std::cout<<"argumentos insuficientes!"<<std::endl;
			return RES_INSUFFICIENT_ARGUMENTS;
		}
		resultado= this->agregar_linea(args[NOMBRE_ARCHIVO] , args[TEXTO]);
		if(!resultado)
			std::cout<<"archivo no existe!"<<std::endl;
		return resultado;
	}

	if(comando == MOSTRAR_ARCHIVO){
		resultado =this->mostrar_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== RES_FILE_DOESNT_EXIST)
			std::cout<<"archivo no existe!"<<std::endl;
		return resultado;
	}

	if(comando == HALLAR_LINEAS){
		if(argc < 4){
			std::cout<<"argumentos insuficientes!"<<std::endl;
			return RES_INSUFFICIENT_ARGUMENTS;
		}
		resultado= this->hallar_lineas(args[NOMBRE_ARCHIVO] , args[TEXTO]);
		if(resultado== LINEAS_NO_HALLADAS){
			std::cout<<"lineas no estan!"<<std::endl;
			return resultado;
		}

		return resultado;
	}

	if(comando == ELIMINAR){
		resultado= this->eliminar_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== RES_FILE_DOESNT_EXIST)
			std::cout<<"archivo no existe!"<<std::endl;
		return resultado;
	}
	
	return RES_OK;


}/*maneja la clase de acuerdo a los argumentos de usuario pasados*/
