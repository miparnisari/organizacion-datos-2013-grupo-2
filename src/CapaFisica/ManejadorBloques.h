/*
 * ManejadorBloques.h
 *
 *  Created on: Mar 25, 2013
 *      Author: martin
 */

#ifndef MANEJADORBLOQUES_H_
#define MANEJADORBLOQUES_H_

#include "ManejadorArchivos.h"
#include "Bloque.h"
#include <stdio.h>
#include "../Utilitarios/Utilitarios.h"

using namespace std;

struct mb_header
{
	unsigned int tamanioBloque;
	unsigned int cantidadBloques;
	int proximoBloqueLibre;
};

class ManejadorBloques:public ManejadorArchivos {

	private:
		struct mb_header header;
		FILE* file_handler;

		int __set_header();
		int __get_header();
		int __marcar_libre_bloque(std::string nombreArchivo, unsigned int numBloque);
		int __marcar_usado_bloque(std::string nombreArchivo, unsigned int numBloque);

	public:
		ManejadorBloques(unsigned int tamBloque = BLOQUE_TAM_DEFAULT);
		~ManejadorBloques();
		
		int crear_archivo(std::string nombreArchivo);
		int abrir_archivo(std::string nombreArchivo, std::string modo);

		int actualizar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque);
		int obtener_bloque(std::string nombreArchivo,  Bloque* bloque, unsigned int numBloque);

		int get_cantidad_bloques();
		int get_proximo_bloque_libre();
		int get_tamanio_bloque();

};

#endif /* MANEJADORBLOQUES_H_ */
