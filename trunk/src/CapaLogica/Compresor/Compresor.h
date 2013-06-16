#ifndef COMPRESOR_H
#define COMPRESOR_H

#include "../../Constantes.h"

class Compresor
{
	public:
		Compresor();
		virtual ~Compresor();

		virtual int comprimir_todo(const char* buffer, const int tamanioBuffer , char* resultado);
	
	private:
		/* add your private declarations */
};

#endif /* COMPRESOR_H */ 
