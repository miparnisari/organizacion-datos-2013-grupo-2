#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "Aritmetico.h"
#include "TablaModelos.h"
#include "TablaOrdenes.h"

#include <map>

#include "../../Utilitarios/Utilitarios.h"
#include "../../Utilitarios/Logger.h"
#include "../../Constantes.h"

using std::stringstream;

class PPMC : public Compresor
{
	public:
		PPMC(Uint orden);
		~PPMC();

		virtual int comprimir_todo
			(const char* buffer,
			const unsigned int tamanioBuffer,
			char* resultado);

		virtual int descomprimir_todo
			(char* bufferComprimido,
			unsigned int tamanioBufferComprimido,
			char* bufferDescomprimido,
			unsigned int cantidadCaracteresOriginal);

	private:
		Uint orden_maximo;
		TablaOrdenes* mapa_ordenes;
		Aritmetico* comp_aritmetico;

		void _reset();

		string _calcular_contexto (Uint simbolo, std::string maximo_contexto_actual);

		int _comprimir (const Uint simbolo, int orden, std::string contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
				vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido, bool esUltimo);

		void _comprimir_un_caracter(int& orden, Uint indiceSimbolo,
				Uint simbolo ,
				string& contexto,
				BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
				vector<bool>& bits_a_emitir,
				char* bufferComprimido,
				Uint& indiceBufferComprimido,
				bool esUltimo);

		int _descomprimir( unsigned long valor, string contextoActual, int ordenActual, Uint& simbolo,
				BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& bufferBits,Aritmetico& aritmeticoCopia);

		void _imprimir_emision(int orden,double probabilidad,Uint simbolo);

		void _guardar_bits(char* bufferComprimido,
			Uint & indiceBufferComprimido,
			BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
			vector<bool> bits_a_emitir, bool esUltimo);

};

#endif /* COMPRESORPPMC_H */ 
