/*
 * ManejadorRegistrosVariables.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "ManejadorRegistrosVariables.h"

using std::stringstream;

int ManejadorRegistrosVariables::agregar_registro(std::string nombreArchivo,std::string dato)const throw(){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	ofstream archivo(nombreArchivo.c_str(), ios::app);
	string datoString= dato;
	unsigned int tamanioDato= datoString.length();

	//archivo<<tamanioDato;
	archivo.write( (char*)(&tamanioDato),sizeof(tamanioDato) );
	archivo.write(dato.c_str(),tamanioDato);

	archivo.close();

	return tamanioDato;

}/*agrega un registro de longitud variable y retorna tamanio del dato escrito*/


int ManejadorRegistrosVariables::registrar_archivo(std::string nombreArchivoDestino,std::string nombreArchivoFuente)
	const throw(){

	if(!archivo_existe(nombreArchivoDestino) || !archivo_existe(nombreArchivoFuente))
		return RES_FILE_DOESNT_EXIST;

	ofstream archivoDestino(nombreArchivoDestino.c_str() , ios::app);
	ifstream archivoFuente(nombreArchivoFuente.c_str() , ios::binary);

	while(archivoFuente.good()){

		int tamanioBuffer;
		//archivoFuente>>tamanioBuffer;
		archivoFuente.read( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );

		if(archivoFuente.good()){
			char* buffer = new char[tamanioBuffer];
			archivoFuente.read(buffer,tamanioBuffer);
			//archivoDestino<<tamanioBuffer;
			archivoDestino.write( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );
			archivoDestino.write(buffer,tamanioBuffer);
			delete[] buffer;
		}

	}

	return RES_OK;


}/*registra todos los registros del archivo fuente en el archivo destino*/


int ManejadorRegistrosVariables::mostrar_archivo(std::string nombreArchivo){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	ifstream archivo(nombreArchivo.c_str(),ios::binary);

	while(archivo.good()){

		int tamanioBuffer;
		//archivo>>tamanioBuffer;
		archivo.read( (char*)(&tamanioBuffer),sizeof(tamanioBuffer) );

		if(archivo.good()){

			char* buffer = new char[tamanioBuffer];
			archivo.read(buffer,tamanioBuffer);

			stringstream stream(ios::in | ios::out);
			stream.write(buffer,tamanioBuffer);
			stream<<endl;
			stream.seekg(0,ios::beg);
			cout<<stream.rdbuf();
			delete[] buffer;

		}

	}

	return RES_OK;

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

	return RES_OK;*/

}/*muestra el archivo por pantalla*/


int ManejadorRegistrosVariables::get_registro(std::string nombreArchivo,RegistroVariable* registro,
		unsigned int numeroRegistro)
{

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;
	if(!registro)
		return RES_ERROR;

	unsigned int cuentaRegistro= 0;
	ifstream archivo(nombreArchivo.c_str());
	unsigned int tamanioBuffer;
	char* buffer;
	bool archivoEstaBien= archivo.good();

	if(numeroRegistro)
	while(archivo.good() && cuentaRegistro<numeroRegistro){
		//archivo>>tamanioBuffer;
		archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
		archivoEstaBien= archivo.good();

		if(archivoEstaBien){
			char* bufAux = new char[tamanioBuffer];
			archivo.read(bufAux,tamanioBuffer);
			cuentaRegistro++;
			delete[] bufAux;
		}

	}

	if(!archivoEstaBien)
		return RES_ERROR;

	//archivo>>tamanioBuffer;
	archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
	buffer= new char[tamanioBuffer];
	archivo.read(buffer,tamanioBuffer);
	registro->limpiar_buffer();
	int resultado= registro->agregar_datos(buffer,tamanioBuffer);

	delete[] buffer;

	return resultado;


}/*guarda los datos en registro*/


int ManejadorRegistrosVariables::mostrar_registro(std::string nombreArchivo,unsigned int numeroRegistro){

	RegistroVariable rv;
	int resultado;

	resultado= get_registro(nombreArchivo,&rv,numeroRegistro);
	if(resultado== RES_ERROR)
		return RES_ERROR;

	rv.mostrar();
	return RES_OK;

}/*muestra un registro especifico*/


int ManejadorRegistrosVariables::mostrar_offsets(std::string nombreArchivo)throw(){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	ifstream archivo(nombreArchivo.c_str(),ios::binary);
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
			char* buffer = new char[tamanioBuffer];
			archivo.read(buffer,tamanioBuffer);
			delete[] buffer;

		}

	}

	return RES_OK;

}/*muestra para cada registro en el archivo su numero de posicion logica y el byte offset del mismo*/


int ManejadorRegistrosVariables::buscar_registro_por_texto(std::string nombreArchivo,std::string texto)throw(){

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

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

		posicionTexto= lineaRegistro.find(texto.c_str());

		if(posicionTexto!= TEXTO_NO_HALLADO){
			cout<<"Registro: "<<cuentaRegistro<<endl;
			cout<<"Posicion: "<<posicionTexto<<endl<<endl;
			algunRegistroHallado= true;
		}

		cuentaRegistro++;

	}

	if(!algunRegistroHallado)
		cout<<"No se hallaron registros"<<endl;

	return RES_OK;

}/*muestra registros que tienen la linea de texto buscada*/


int ManejadorRegistrosVariables::mostrar_caracteristicas_registro(std::string nombreArchivo,unsigned int numeroRegistro){

	RegistroVariable rv;
	int resultado;

	resultado= get_registro(nombreArchivo,&rv,numeroRegistro);
	if(!resultado)
		return RES_ERROR;

	ifstream archivo(nombreArchivo.c_str());
	unsigned int cuentaBytes= 0;
	unsigned int cuentaRegistro= 0;
	unsigned int tamanioBuffer;

	while(cuentaRegistro!=numeroRegistro){

		char* buffer = new char[tamanioBuffer];
		//archivo>>tamanioBuffer;
		archivo.read((char*)(&tamanioBuffer),sizeof(tamanioBuffer));
		cuentaBytes+= sizeof(int);
		archivo.read(buffer,tamanioBuffer);
		cuentaBytes+= tamanioBuffer;
		cuentaRegistro++;
		delete[] buffer;
	}

	cout<<"Registro: "<<numeroRegistro<<endl;
	cout<<"Tamanio: "<<rv.get_tamanio()<<endl;

	return RES_OK;

}


int ManejadorRegistrosVariables::manejar(int argc,char** args){

	return RES_OK;
}

