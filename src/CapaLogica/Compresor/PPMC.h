#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "../../Constantes.h"
#include "Aritmetico.h"
#include "Contextos.h"
#include "../../Utilitarios/Utilitarios.h"
#include <map>

class PPMC : public Compresor
{
	public:
		PPMC(unsigned short orden);
		~PPMC();

		void _guardar_bits(char* bufferComprimido,
				Uint & indiceBufferComprimido,
				BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
				vector<bool> bits_a_emitir);

		std::vector<bool> _comprimir_ultimo_paso(string contexto);
	
	private:
		unsigned short orden_maximo;
		Contextos* contextos;
		Aritmetico* comp_aritmetico;

		void _crear_modelo_vacio (string nombre_modelo);

		int comprimir (const Uint simbolo, std::string contexto_del_simbolo, std::vector<bool>& a_emitir);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

};

#endif /* COMPRESORPPMC_H */ 
