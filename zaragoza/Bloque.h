/*
 * Bloque.h
 *
 *  Created on: Mar 25, 2013
 *      Author: martin
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_

#include<fstream>
#include<sstream>
#include"RegistroVariable.h"
#include <climits>


using namespace std;

class Bloque {

	protected:
		char* bufferBloque;
		unsigned int espacioLibre/*espacio disponible en buffer. Esta variable ocupa espacio en el buffer*/;
		unsigned int tamanioBloque;
		unsigned int espacioLibreOffset;

		virtual void obtener_espacio_libre()throw();
		virtual void escribir_espacio_libre()throw();
		virtual unsigned int obtener_offset_final()throw();
		virtual unsigned int obtener_offset_registro(unsigned short numeroRegistro)throw();



	public:
		static const unsigned int TAMANIO_BLOQUE_DEFAULT= 1024*4;/*4Kb*/
		static const unsigned int OFFSET_INVALIDO= UINT_MAX;

		static const int DATO_NULO= -1;
		static const int ESPACIO_INSUFICIENTE= 0;
		static const int EXITO= 1;
		static const int BLOQUE_VACIO= -2;
		static const int FRACASO= 0;

		virtual bool esta_vacio()throw();
		virtual void cargar(char* datos)throw();
		virtual void limpiar()throw();
		virtual unsigned int get_espacio_libre()throw(){return espacioLibre;}
		virtual int agregar_registro(char* dato,unsigned int tamanioDato)throw();
		virtual int agregar_registro(RegistroVariable* registro)throw();
		virtual int remover_registro(unsigned short numeroRegistro)throw();
		virtual int recuperar_registro(RegistroVariable* registro,unsigned short numeroRegistro)throw();
		virtual unsigned short contar_registros()throw();
		virtual void exportar_bloque(char* copia)throw();
		virtual int guardar(ostream& salida)throw();
		virtual void listar_registros()throw();
		unsigned int get_tamanio_bloque()throw(){return this->tamanioBloque;}

		virtual unsigned int calcular_espacio_usable()throw();
		virtual unsigned int calcular_espacio_ocupado()throw();

		virtual unsigned int calcular_espacio_libre()throw();
		/*este metodo puede ser alterado en clases hijas para agregar un colchon de amortiguacion de
		 * datos de 75% por ejemplo*/

		Bloque(unsigned int tb){
			if(tb>= TAMANIO_BLOQUE_DEFAULT)
				this->tamanioBloque= tb;
			else
				this->tamanioBloque= TAMANIO_BLOQUE_DEFAULT;
			espacioLibre= tamanioBloque - sizeof(espacioLibre);
			espacioLibreOffset= espacioLibre;
			bufferBloque= new char[tamanioBloque];
			limpiar();
		}/*constructor, setea el tamanio del buffer con el valor tamanioBloque, el mismo esta medido
		en bytes.*/
		Bloque(){
			tamanioBloque= TAMANIO_BLOQUE_DEFAULT;
			espacioLibre= tamanioBloque - sizeof(espacioLibre);
			espacioLibreOffset= espacioLibre;
			bufferBloque= new char[tamanioBloque];
			limpiar();
		}
		virtual ~Bloque(){
			delete[] bufferBloque;
		}

};/*Bloque*/
/*esta diseniado para almacenar registros de tamanio mayor a un mega dado que almacena los tamanios
 * de los registros variables como enteros de 4 bytes.*/

#endif /* BLOQUE_H_ */
