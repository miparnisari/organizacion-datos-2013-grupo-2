#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "../../Constantes.h"
#include "Aritmetico.h"
#include "Orden.h"
#include "../../Utilitarios/Utilitarios.h"
#include <map>

class PPMC : public Compresor
{
	public:
		PPMC(unsigned short orden);
		~PPMC();

		int comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir);
		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

		void comprimir_un_caracter(int& orden, Uint indiceSimbolo, Uint simbolo , string& contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
					vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido,bool esUltimo);

		int descomprimir( unsigned long valor, string contextoActual, int ordenActual, Uint& simbolo,
				BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& bufferBits,Aritmetico& aritmeticoCopia);

		int descomprimir_todo
			(char* bufferComprimido,
			int tamanioBufferComprimido,
			char* bufferDescomprimido,
			unsigned int precision,
			unsigned int cantidadCaracteresOriginal);

	private:
		unsigned short orden_maximo;
		map<int,Orden*> mapa_ordenes;
		Aritmetico* comp_aritmetico;
		string contexto;

		void _inicializar_orden_menosuno();
		void _inicializar_orden_cero();

		void _comprimir_ultimo (std::vector<bool>& a_emitir);

		void _actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo);

		void _imprimir_todos_ordenes();

		void _crear_modelo_vacio (int orden, string nombre_modelo);

		void _comprimir_ultimo_paso(Uint simbolo, int orden, string contexto,
				char* bufferComprimido, Uint indiceBufferComprimido, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits);

		void _guardar_bits(char* bufferComprimido,
			Uint & indiceBufferComprimido,
			BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
			vector<bool> bits_a_emitir);

		void _emitir_completando_octeto(char* bufferComprimido,
				Uint & indiceBufferComprimido,
				BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
				vector<bool> bits_a_emitir);

};

#endif /* COMPRESORPPMC_H */ 