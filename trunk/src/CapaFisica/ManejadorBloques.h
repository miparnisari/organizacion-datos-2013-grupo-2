#ifndef MANEJADORBLOQUES_H_
#define MANEJADORBLOQUES_H_

#include "ManejadorArchivos.h"
#include "Bloque.h"
#include <stdio.h>
#include "../Utilitarios/Utilitarios.h"

struct mb_header
{
	unsigned int tamanioBloque;
	unsigned int cantidadBloques;
	int proximoBloqueLibre;
};

class ManejadorBloques:public ManejadorArchivos {

	private:
		struct mb_header 	header;
		FILE* 				file_handler;
		std::string			nombreArchivo;

		int __set_header();
		int __get_header();
		bool __num_bloque_es_valido	(unsigned int numBloque);
		bool __es_tope_de_pila		(int numBloque);
		int __usar_bloque			(Bloque* bloque, unsigned int numBloque);
		int __agregar_bloque_al_final();
		int __escribir_bloque		(const Bloque* bloque, unsigned int offset);
		int __liberar_bloque		(Bloque* bloque, unsigned int numBloque);
		unsigned int __get_primer_bloque_libre	();

	public:
		ManejadorBloques();
		~ManejadorBloques();
		
		unsigned int get_cantidad_bloques();
		unsigned int get_tamanio_bloque();
		void set_ruta(std::string ruta);
		std::string get_ruta();

		/* Crea un archivo de bloques nuevo, sin bloques para usar.
		Si el archivo ya existia, sus contenidos se pisaran.
		POSTCONDICION: cierra el archivo. */
		int crear_archivo(std::string nombreArchivo, unsigned int tamBloque);


		int eliminar_archivo(std::string nombreArchivo);

		/* Abre un archivo con un modo de apertura especifico.
		Para info sobre los modos posibles: http://www.cplusplus.com/reference/cstdio/fopen/
		*/
		int abrir_archivo(std::string nombreArchivo, std::string modo);


		int cerrar_archivo();

		int escribir_bloque(Bloque* bloque);
		/* Inserta un bloque en la primera posicion libre del archivo.
		 * Si no hay espacios libres, hace un "append" del bloque al final del archivo.
		 * Devuelve el numero de bloque escrito, o RES_ERROR si falló. */


		/* Devuelve un bloque del archivo.
		PRECONDICION: el archivo se debe abrir en modo escritura.
		POSTCONDICION: se lo debe cerrar. */
		int sobreescribir_bloque(Bloque* bloque, unsigned int numBloque);

		/* Devuelve un bloque vacio con sus atributos ya seteados.
		 * RECORDAR: liberar la memoria del mismo con delete luego de usarlo.
		 */
		Bloque* crear_bloque();

		/* Lee un bloque no libre del disco y lo guarda en un buffer "bloque" para operar sobre el.
		PRECONDICION: el archivo se debe abrir en modo lectura.
		POSTCONDICION: se lo debe cerrar. Liberar la memoria del Bloque.*/
		Bloque* obtener_bloque(unsigned int numBloque);

};

#endif /* MANEJADORBLOQUES_H_ */
