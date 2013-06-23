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

		void _actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo);
		int comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

	
	private:
		unsigned short orden_maximo;
		map<int,Contextos*> mapa_ordenes;
		Aritmetico* comp_aritmetico;

		void _imprimir_todos_ordenes();

		void _crear_modelo_vacio (int orden, string nombre_modelo);

		std::vector<bool> _comprimir_ultimo_paso(int orden, string contexto);

		void _guardar_bits(char* bufferComprimido,
			Uint & indiceBufferComprimido,
			BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
			vector<bool> bits_a_emitir);

};

#endif /* COMPRESORPPMC_H */ 
