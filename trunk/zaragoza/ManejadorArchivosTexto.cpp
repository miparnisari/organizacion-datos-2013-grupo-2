/*
 * ManejadorArchivosTexto.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: martin
 */

#include "ManejadorArchivosTexto.h"




int ManejadorArchivosTexto::agregar_linea(char* nombreArchivo,char* linea)const throw(){

	ifstream archivoLectura;
	archivoLectura.open(nombreArchivo,ios::in);
	if(!archivoLectura.good())
		return ARCHIVO_NO_EXISTE;

	archivoLectura.seekg(0,ios::end);
	archivoLectura.close();

	ofstream archivoEscritura(nombreArchivo,ios::app);
	archivoEscritura<<linea;
	archivoEscritura<<'\n';

	archivoEscritura.close();
	return EXITO;

}/*agrega linea al final de un archivo existente*/


int ManejadorArchivosTexto::mostrar_archivo(char* nombreArchivo){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	ifstream archivo(nombreArchivo,ios::in);
	const unsigned short TAMANIO_BUFFER= 1024;
	char* buffer= new char[TAMANIO_BUFFER];

	while(archivo.good()){

		archivo.getline(buffer,TAMANIO_BUFFER);
		if(archivo.good()){
			cout<<buffer<<endl;
		}

	}

	delete[] buffer;
	return EXITO;

}/*muestra lineas de un archivo por pantalla*/


int ManejadorArchivosTexto::hallar_lineas(char* nombreArchivo,char* linea)const throw(){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	const unsigned short TAMANIO_BUFFER_CHAR= 1024;
	char* bufferChar= new char[TAMANIO_BUFFER_CHAR];
	ifstream archivo(nombreArchivo,ios::in);
	const int LINEA_NO_HALLADA= -1;
	int posicionLinea= LINEAS_NO_HALLADAS;
	string lineaString= linea;

	while(archivo.good()){

		archivo.getline(bufferChar,TAMANIO_BUFFER_CHAR);

		if(archivo.good()){
			string bufferString= bufferChar;
			posicionLinea= bufferString.find(lineaString);
			if(posicionLinea!= LINEAS_NO_HALLADAS)
				cout<<bufferString<<endl;
		}

	}

	archivo.seekg(0,ios::end);
	archivo.close();

	delete[] bufferChar;
	return posicionLinea;

}/*halla lineas del archivo de texto donde se encuentra una linea especifica*/


int ManejadorArchivosTexto::manejar(int argc,char** args){

	if(argc== 1 || argc== 2){
		cout<<"argumentos insuficientes!"<<endl;
		return ARGUMENTOS_INSUFICIENTES;
	}
	/*sin argumentos suficientes, error, no se sabe que hacer*/

	enum EnumeracionArgumentos {PROGRAMA=0 , NOMBRE_ARCHIVO , COMANDO , TEXTO};
	string const CREAR= "-c";
	string const REGISTRAR= "-r";
	string const MOSTRAR_ARCHIVO= "-l";
	string const HALLAR_LINEAS= "-s";
	string const ELIMINAR= "-e";

	string comando= args[COMANDO];

	int resultado;

	if(comando == CREAR){
		resultado= this->crear_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== FRACASO)
			cout<<"error en creacion!"<<endl;
		return resultado;
	}

	if(comando == REGISTRAR){
		if(argc < 4){
			cout<<"argumentos insuficientes!"<<endl;
			return ARGUMENTOS_INSUFICIENTES;
		}
		resultado= this->agregar_linea(args[NOMBRE_ARCHIVO] , args[TEXTO]);
		if(!resultado)
			cout<<"archivo no existe!"<<endl;
		return resultado;
	}

	if(comando == MOSTRAR_ARCHIVO){
		resultado =this->mostrar_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== ARCHIVO_NO_EXISTE)
			cout<<"archivo no existe!"<<endl;
		return resultado;
	}

	if(comando == HALLAR_LINEAS){
		if(argc < 4){
			cout<<"argumentos insuficientes!"<<endl;
			return ARGUMENTOS_INSUFICIENTES;
		}
		resultado= this->hallar_lineas(args[NOMBRE_ARCHIVO] , args[TEXTO]);
		if(resultado== LINEAS_NO_HALLADAS){
			cout<<"lineas no estan!"<<endl;
			return resultado;
		}

		return resultado;
	}

	if(comando == ELIMINAR){
		resultado= this->eliminar_archivo(args[NOMBRE_ARCHIVO]);
		if(resultado== ARCHIVO_NO_EXISTE)
			cout<<"archivo no existe!"<<endl;
		return resultado;
	}


}/*maneja la clase de acuerdo a los argumentos de usuario pasados*/
