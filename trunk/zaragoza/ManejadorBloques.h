/*
 * ManejadorBloques.h
 *
 *  Created on: Mar 25, 2013
 *      Author: martin
 */

#ifndef MANEJADORBLOQUES_H_
#define MANEJADORBLOQUES_H_

#include"ManejadorArchivos.h"
#include"Bloque.h"
#include<sstream>
#include<fstream>

using namespace std;

class ManejadorBloques:public ManejadorArchivos {

	protected:
		unsigned int tamanioBloque;

		virtual int hallar_bloque_libre(ifstream& archivo,Bloque* bloque,unsigned int espacioNecesario)throw();

	public:
		static const int ERROR= -1;
		static const int TODOS_BLOQUES_OCUPADOS= -1;

		virtual int contar_bloques(char* nombreArchivo)throw();
		virtual int agregar_registro(char* nombreArchivo,char* dato,unsigned int tamanioDato)throw();
		virtual int agregar_archivo(char* nombreArchivoDestino,char* nombreArchivoFuente)throw();
		virtual bool esta_vacio(char* nombreArchivo)throw();
		virtual int listar_archivo_completo(char* nombreArchivo)throw();
		virtual int obtener_bloque(char* nombreArchivo,unsigned short numeroBloque,Bloque* bloque)throw();
		virtual int listar_bloque(char* nombreArchivo,unsigned short numeroBloque)throw();

		ManejadorBloques(unsigned int tb){
			tamanioBloque= tb;
		}
		virtual ~ManejadorBloques(){}


};

#endif /* MANEJADORBLOQUES_H_ */
