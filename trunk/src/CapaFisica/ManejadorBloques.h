#ifndef MANEJADORBLOQUES_H_
#define MANEJADORBLOQUES_H_

#include "ManejadorArchivos.h"
#include "Bloque.h"
#include <stdio.h>
#include "../Utilitarios/Utilitarios.h"

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
		bool __num_bloque_es_valido(unsigned int numBloque);
		bool __es_tope_de_pila(std::string nombreArchivo, unsigned numBloque);
		int __usar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque);
		int __agregar_bloque_al_final(std::string nombreArchivo);
		int __escribir_bloque(std::string nombreArchivo, const Bloque* bloque, unsigned int offset);
		int __liberar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque);
		unsigned int __get_primer_bloque_libre(std::string nombreArchivo);

	public:
		ManejadorBloques(unsigned int tamBloque, unsigned int min, unsigned int max);
		~ManejadorBloques();
		
		int crear_archivo(std::string nombreArchivo);
		int eliminar_archivo(std::string nombreArchivo);

		int abrir_archivo(std::string nombreArchivo, std::string modo);
		int cerrar_archivo();

		int escribir_bloque(std::string nombreArchivo, Bloque* bloque);
		/* Inserta un bloque en la primera posicion libre del archivo.
		 * Si no hay espacios libres, hace un "append" del bloque al final del archivo.
		 * Devuelve el numero de bloque escrito, o RES_ERROR si falló. */
		int sobreescribir_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque);
		/* El bloque referenciado por "numBloque" se lo sobreescribe con los contenidos del nuevo bloque.
		 */
		Bloque* obtener_bloque(std::string nombreArchivo, unsigned int numBloque);
		/* Lee un bloque no libre del disco y lo guarda en un buffer "bloque" para operar sobre el. */

		unsigned int get_cantidad_bloques();
		int get_primer_bloque_libre();
		unsigned int get_tamanio_bloque();
		unsigned int get_max_regs_bloque();
		unsigned int get_min_regs_bloque();

};

#endif /* MANEJADORBLOQUES_H_ */
