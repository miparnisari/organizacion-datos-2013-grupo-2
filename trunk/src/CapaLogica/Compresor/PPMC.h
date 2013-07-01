#ifndef COMPRESORPPMC_H
#define COMPRESORPPMC_H

#include "Compresor.h"
#include "../../Constantes.h"
#include "Aritmetico.h"
#include "Orden.h"
#include "../../Utilitarios/Utilitarios.h"
#include "../../Utilitarios/Logger.h"
#include <map>
#include <fstream>

using std::stringstream;

class PPMC : public Compresor
{
	public:
		PPMC(unsigned int orden);
		~PPMC();

		virtual int comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado);

		virtual int descomprimir_todo
			(char* bufferComprimido,
			unsigned int tamanioBufferComprimido,
			char* bufferDescomprimido,
			unsigned int cantidadCaracteresOriginal);

	private:
		int orden_maximo;
		map<int,Orden*> mapa_ordenes;
		Aritmetico* comp_aritmetico;

		void _limpiar_ordenes();

		void _reset();

		void _inicializar();
		void _inicializar_orden_menosuno();
		void _inicializar_orden_cero();

		int _comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir);

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

		void _comprimir_ultimo (std::vector<bool>& a_emitir);

		void _actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo);

		void _imprimir_todos_ordenes();
		void _imprimir_emision(int orden,double probabilidad,Uint simbolo);

		void _crear_modelo_vacio (int orden, string nombre_modelo);

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