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
#include <climits>

using namespace std;

typedef unsigned short TamanioBitset;



template<TamanioBitset tamanioBuffer>
class BufferBits{

	private:
		bitset<tamanioBuffer>* buffer;
		/*se debe contemplar que los bits en el bitset se agregan de derecha a izquierda*/
		TamanioBitset bitActual;
		/*es el indice posicional del proximo bit a insertar. Se incrementa a medida que se llama a agregar_bit*/

		string _invertir_string(string s){

			if(s.length()== 0)
				return "";

			const unsigned short TAMANIO_STRING= s.length();
			string retornar(TAMANIO_STRING,'-');

			for(unsigned short i=0;i<TAMANIO_STRING;i++){
				char c= s.at(i);
				unsigned short indice= TAMANIO_STRING-1-i;
				retornar[indice]= c;
			}


			return retornar;

		}


	public:


		class TamanioBufferIncorrecto:public exception{

		public:
			const char* what()const throw() {
				return "ExcepcionTamanioBufferIncorrecto";
			}
		};


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


		void flush(){

			this->reset();
			this->bitActual= 0;

		}

		void reset(){
			buffer->reset();
		}/*setea todos los bits en 0*/
		void set(){
			buffer->set();
		}/*setea todos los bits en 1*/


		int agregar_bit(bool bit){

			if(bitActual>= tamanioBuffer)
				return RES_ERROR;

			buffer->set(bitActual,bit);
			bitActual++;

			return RES_OK;

		}
		int agregar_bit(int bit){

			if(bit)
				return agregar_bit(true);
			return agregar_bit(false);

		}
		int agregar_bits(string s){

			const unsigned short TAMANIO_STRING= s.length();
			if(TAMANIO_STRING + bitActual > tamanioBuffer )
				return RES_ERROR;

			for(unsigned short i=0;i<TAMANIO_STRING;i++){
				char c= s.at(i);
				if(c!= '1' && c!= '0')
					return RES_ERROR;

				bool b= (c=='1');
				this->agregar_bit(b);
			}

			return RES_OK;

		}
		int agregar_bits(bool* valores,unsigned short tamanioValores){

			if(tamanioValores+bitActual > tamanioBuffer)
				return RES_ERROR;
			if(!tamanioValores)
				return RES_ERROR;

			for(unsigned short i=0;i<tamanioValores;i++)
				this->agregar_bit(valores[i]);

			return RES_OK;

		}
		int get_bit(unsigned short posicion,bool& bit){

			if(posicion>= bitActual)
				return RES_ERROR;

			string bufferString= to_string();
			char c= bufferString.at(posicion);

			bit= (c=='1');
			return RES_OK;

		}/*retorna el valor del bit en la posicion deseada*/


		string to_string(){

			if(bitActual== 0)
				return string("");

			string retornar;
			string bufferString= buffer->to_string();


			unsigned short i=tamanioBuffer-1;
			unsigned short indice= 0;

			while(true){

				char c= bufferString.at(i);
				retornar.push_back(c);

				i--;
				indice++;
				if(indice>=bitActual)
					break;

			}


			return retornar;

		}/*retorna un string con los bits insertados en el orden apropiado*/
		string to_string_bitset(){

			return buffer->to_string();

		}/*se retorna el objeto string creado por el buffer (bitset) . Dicho string tendra los bits agregados en el orden
		invertido al agregado*/




		TamanioBitset get_indice_buffer(){
			return this->bitActual;
		}/*retorna el valor bitActual, es decir la cantidad de bits que se agregaron.*/
		unsigned short get_cantidad_bytes(){
			unsigned short resultado;
			resultado= (unsigned short)(bitActual/8);
			return resultado;
		}/*retorna la cantidad de bytes completos en buffer*/


		int to_char(){

			const unsigned short TAMANIO_CHAR= 8;
			if(tamanioBuffer != TAMANIO_CHAR)
				return RES_ERROR;

			string bufferString= this->to_string();
			string bufferStringInvertido= _invertir_string(bufferString);
			bitset<TAMANIO_CHAR> bitsetAuxiliar;

			for(unsigned short i=0;i<TAMANIO_CHAR;i++){

				char c= bufferStringInvertido.at(i);
				bool b= c=='1';
				bitsetAuxiliar.set(i,b);

			}

			return (int)bitsetAuxiliar.to_ulong();

		}/*devuelve un valor entero que representa el caracter guardado en bits. retorna RES_ERROR en caso que
		tamanioBuffer sea != a 8 bits*/


		unsigned char get_byte(unsigned short numeroByte)throw(TamanioBufferIncorrecto){

			const unsigned short CANTIDAD_BYTES= get_cantidad_bytes();
			if(numeroByte>= CANTIDAD_BYTES)
				return UCHAR_MAX;
			if(tamanioBuffer< 8){
				TamanioBufferIncorrecto tbi;
				throw tbi;
			}


			BufferBits<8> bb;

			string bs= to_string();
			unsigned short bitNumeroByte= 8*numeroByte;

			for(unsigned short i=0;i<8;i++){

				unsigned short indice= bitNumeroByte+i;
				char c= bs.at(indice);
				bool b= c=='1';
				bb.agregar_bit(b);

			}

			unsigned char retornar= (unsigned char)bb.to_char();


			return retornar;

		}/*retorna el byte en la posicion numeroByte. Los mismos se cuentan desde el 0*/


};






