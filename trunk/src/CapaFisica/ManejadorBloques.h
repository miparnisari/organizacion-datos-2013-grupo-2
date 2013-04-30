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
	unsigned int minRegsBloque;
	unsigned int maxRegsBloque;
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
		ManejadorBloques(unsigned int tamBloque, unsigned int min, unsigned int max);
		~ManejadorBloques();
		
		int crear_archivo(std::string nombreArchivo);
		int eliminar_archivo(std::string nombreArchivo);

		int abrir_archivo(std::string nombreArchivo, std::string modo);
		int cerrar_archivo();

		int eliminar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, unsigned int numReg);
		int agregar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, RegistroVariable* reg);
		int get_registro_de_bloque(std::string nombreArchivo, unsigned int numReg, unsigned int numBloque, RegistroVariable* reg);

		int actualizar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque);
		int obtener_bloque(std::string nombreArchivo,  Bloque* bloque, unsigned int numBloque);

		unsigned int get_cantidad_bloques();
		int get_proximo_bloque_libre();
		unsigned int get_tamanio_bloque();
		unsigned int get_max_regs_bloque();
		unsigned int get_min_regs_bloque();

};

#endif /* MANEJADORBLOQUES_H_ */
