#ifndef COMPRESOR_H
#define COMPRESOR_H

#include "../../Constantes.h"

class Compresor
{
	public:
		Compresor();
		virtual ~Compresor();

		virtual int comprimir_todo
			(const char* buffer,
			const unsigned int tamanioBuffer,
			char* resultado) = 0;
		virtual int descomprimir_todo
			(char* buffer,
			unsigned int tamanio,
			char* descomprimido,
			unsigned int cantidadCaracteresOriginal) = 0;
	
	private:
		/* add your private declarations */
};

#endif /* COMPRESOR_H */ 
