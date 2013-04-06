/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;

int ManejadorRegistrosVariables::agregar_registro(char* nombreArchivo,char* dato)const throw(){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	ofstream archivo(nombreArchivo , ios::app);
	string datoString= dato;
	unsigned int tamanioDato= datoString.length();

	//archivo<<tamanioDato;
	archivo.write( (char*)(&tamanioDato),sizeof(tamanioDato) );
	archivo.write(dato,tamanioDato);

	archivo.close();

	return tamanioDato;

}/*agrega un registro de longitud variable y retorna tamanio del dato escrito*/


int ManejadorRegistrosVariables::registrar_archivo(char* nombreArchivoDestino,char* nombreArchivoFuente)
	const throw(){

	if(!archivo_existe(nombreArchivoDestino) || !archivo_existe(nombreArchivoFuente))
		return ARCHIVO_NO_EXISTE;

	ofstream archivoDestino(nombreArchivoDestino , ios::app);
	ifstream archivoFuente(nombreArchivoFuente , ios::binary);

	while(archivoFuente.good()){

		int tamanioBuffer;
		//archivoFuente>>tamanioBuffer;
		archivoFuente.read( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );

		if(archivoFuente.good()){
			char buffer[tamanioBuffer];
			archivoFuente.read(buffer,tamanioBuffer);
			//archivoDestino<<tamanioBuffer;
			archivoDestino.write( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );
			archivoDestino.write(buffer,tamanioBuffer);
		}

	}

	return EXITO;


}/*registra todos los registros del archivo fuente en el archivo destino*/


int ManejadorRegistrosVariables::mostrar_archivo(char* nombreArchivo){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	ifstream archivo(nombreArchivo,ios::binary);

	while(archivo.good()){

		int tamanioBuffer;
		//archivo>>tamanioBuffer;
		archivo.read( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );

		if(archivo.good()){

			char buffer[tamanioBuffer];
			archivo.read(buffer,tamanioBuffer);

			stringstream stream(ios::in | ios::out);
			stream.write(buffer,tamanioBuffer);
			stream<<endl;
			stream.seekg(0,ios::beg);
			cout<<stream.rdbuf();

		}

	}

	return EXITO;

	/*stringstream streamDestino(ios::in | ios::out);


	while(archivo.good()){

		unsigned int tamanioBuffer;
		char* buffer;

		archivo>>tamanioBuffer;

		if(archivo.good()){
			buffer= new char[tamanioBuffer];
			archivo.read(buffer,tamanioBuffer);
			streamDestino.write(buffer,tamanioBuffer);
			streamDestino<<'\n';
			delete[] buffer;
		}

	}

	streamDestino.seekg(0,ios::beg);
	cout<<streamDestino.rdbuf();

	return EXITO;*/

}/*muestra el archivo por pantalla*/


int ManejadorRegistrosVariables::get_registro(char* nombreArchivo,RegistroVariable* registro,
		unsigned int numeroRegistro){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;
	if(!registro)
		return FRACASO;

	unsigned int cuentaRegistro= 0;
	ifstream archivo(nombreArchivo);
	unsigned int tamanioBuffer;
	char* buffer;
	bool archivoEstaBien= archivo.good();

	if(numeroRegistro)
	while(archivo.good() && cuentaRegistro<numeroRegistro){
		//archivo>>tamanioBuffer;
		archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
		archivoEstaBien= archivo.good();

		if(archivoEstaBien){
			char bufAux[tamanioBuffer];
			archivo.read(bufAux,tamanioBuffer);
			cuentaRegistro++;
		}

	}

	if(!archivoEstaBien)
		return FRACASO;

	//archivo>>tamanioBuffer;
	archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
	buffer= new char[tamanioBuffer];
	archivo.read(buffer,tamanioBuffer);
	registro->limpiar();
	int resultado= registro->agregar_datos(buffer,tamanioBuffer);

	delete[] buffer;

	return resultado;


}/*guarda los datos en registro*/


int ManejadorRegistrosVariables::mostrar_registro(char* nombreArchivo,unsigned int numeroRegistro){

	RegistroVariable rv;
	int resultado;

	resultado= get_registro(nombreArchivo,&rv,numeroRegistro);
	if(resultado== FRACASO)
		return FRACASO;

	rv.mostrar();
	return EXITO;

}/*muestra un registro especifico*/


int ManejadorRegistrosVariables::mostrar_offsets(char* nombreArchivo)throw(){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	ifstream archivo(nombreArchivo,ios::binary);
	unsigned int offset= 0;
	unsigned int cuentaRegistros= 0;

	while(archivo.good()){

		int tamanioBuffer;
		//archivo>>tamanioBuffer;
		archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));

		if(archivo.good()){

			cout<<"Registro: "<<cuentaRegistros<<" - ";
			cout<<offset;
			offset+= sizeof(int);
			offset+= tamanioBuffer;
			char buffer[tamanioBuffer];
			archivo.read(buffer,tamanioBuffer);

		}

	}

	return EXITO;

}/*muestra para cada registro en el archivo su numero de posicion logica y el byte offset del mismo*/


int ManejadorRegistrosVariables::buscar_registro_por_texto(char* nombreArchivo,char* texto)throw(){

	if(!archivo_existe(nombreArchivo))
		return ARCHIVO_NO_EXISTE;

	RegistroVariable rv;
	unsigned int cuentaRegistro= 0;
	const int TEXTO_NO_HALLADO= -1;
	int posicionTexto= TEXTO_NO_HALLADO;
	string textoString(texto);
	bool algunRegistroHallado= false;

	while(get_registro(nombreArchivo,&rv,cuentaRegistro)){

		stringstream stream(ios::in |ios::out);
		rv.guardar(stream);
		stream<<'\n';
		stream.seekg(0,ios::beg);
		string lineaRegistro= stream.str();

		posicionTexto= lineaRegistro.find(texto);

		if(posicionTexto!= TEXTO_NO_HALLADO){
			cout<<"Registro: "<<cuentaRegistro<<endl;
			cout<<"Posicion: "<<posicionTexto<<endl<<endl;
			algunRegistroHallado= true;
		}

		cuentaRegistro++;

	}

	if(!algunRegistroHallado)
		cout<<"No se hallaron registros"<<endl;

	return EXITO;

}/*muestra registros que tienen la linea de texto buscada*/


int ManejadorRegistrosVariables::mostrar_caracteristicas_registro(char* nombreArchivo,unsigned int numeroRegistro){

	RegistroVariable rv;
	int resultado;

	resultado= get_registro(nombreArchivo,&rv,numeroRegistro);
	if(!resultado)
		return FRACASO;

	ifstream archivo(nombreArchivo);
	unsigned int cuentaBytes= 0;
	unsigned int cuentaRegistro= 0;
	unsigned int tamanioBuffer;

	while(cuentaRegistro!=numeroRegistro){

		char buffer[tamanioBuffer];
		//archivo>>tamanioBuffer;
		archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
		cuentaBytes+= sizeof(int);
		archivo.read(buffer,tamanioBuffer);
		cuentaBytes+= tamanioBuffer;
		cuentaRegistro++;

	}

	cout<<"Registro: "<<numeroRegistro<<endl;
	cout<<"Tamanio: "<<rv.get_tamanio()<<endl;

	return EXITO;

}


int ManejadorRegistrosVariables::manejar(int argc,char** args){

	return EXITO;
}

