#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "../../Constantes.h"
#include "Aritmetico.h"
#include "Contextos.h"
#include <map>

class PPMC : public Compresor
{
	public:
		PPMC(unsigned short orden);
		~PPMC();
	
	private:
		unsigned short orden_maximo;
		Contextos* contextos;
		Aritmetico* comp_aritmetico;

		void _crear_modelo_vacio (string nombre_modelo);

		Uint comprimir (const double proba, std::string contexto_del_simbolo);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

};

#endif /* COMPRESORPPMC_H */ 
