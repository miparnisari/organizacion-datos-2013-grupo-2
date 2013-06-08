/*
 * BufferBits.h
 *
 *  Created on: Jun 8, 2013
 *      Author: martin
 */
#include "../../Constantes.h"
#include <string>
#include <iostream>
#include <bitset>
#include <exception>

using namespace std;

typedef unsigned short TamanioBitset;

template<TamanioBitset tamanioBuffer>
class BufferBits{

	private:
		bitset<tamanioBuffer>* buffer;
		TamanioBitset bitActual;

	public:
		BufferBits()throw(exception){
			if(tamanioBuffer== 0){
				exception e;
				throw(e);
			}

			buffer= new bitset<tamanioBuffer>;
			buffer->reset();
			bitActual= 0;
		}
		~BufferBits(){
			delete buffer;
		}


		void reset(){
			buffer->reset();
		}/*setea todos los bits en 0*/
		void set(){
			buffer->set();
		}/*setea todos los bits en 1*/


		TamanioBitset get_bit_actual(){
			return this->bitActual;
		}
		unsigned short get_cantidad_bytes(){
			unsigned short resultado;
			resultado= (unsigned short)(bitActual/8);
			return resultado;
		}/*retorna la cantidad de bytes completos en buffer*/

};






