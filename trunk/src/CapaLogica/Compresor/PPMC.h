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

		int comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

	private:
		unsigned short orden_maximo;
		map<int,Contextos*> mapa_ordenes;
		Aritmetico* comp_aritmetico;
		string contexto;

		void _comprimir_un_caracter(int& orden,Uint simbolo, string& contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
				vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido);

		void _actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo);

		void _imprimir_todos_ordenes();

		void _crear_modelo_vacio (int orden, string nombre_modelo);

		void _comprimir_ultimo_paso(Uint simbolo, int orden, string contexto,
				char* bufferComprimido, Uint indiceBufferComprimido, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits);

		void _guardar_bits(char* bufferComprimido,
			Uint & indiceBufferComprimido,
			BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
			vector<bool> bits_a_emitir);

};

#endif /* COMPRESORPPMC_H */ 
