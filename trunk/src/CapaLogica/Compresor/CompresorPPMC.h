#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "../../Constantes.h"
#include "CompresorAritmetico.h"
#include "Contexto.h"
#include <map>

class CompresorPPMC : public Compresor
{
	public:
		CompresorPPMC(unsigned short orden);
		~CompresorPPMC();
	
	private:
		unsigned short orden_maximo;
		Contexto* contextos;
		CompresorAritmetico* comp_aritmetico;

		void _crear_modelo_vacio (string nombre_modelo);

		Uint comprimir (const double proba, std::string contexto_del_simbolo);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

};

#endif /* COMPRESORPPMC_H */ 
