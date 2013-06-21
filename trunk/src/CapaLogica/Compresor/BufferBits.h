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

typedef unsigned long long TamanioBitset;
//antes: typedef unsigned short TamanioBitset
const TamanioBitset TAMANIO_BUFFER_BITS_DEFAULT= 128;
const unsigned short BITS_LONG= 32;


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

			const TamanioBitset TAMANIO_STRING= s.length();
			string retornar(TAMANIO_STRING,'-');

			for(TamanioBitset i=0;i<TAMANIO_STRING;i++){
				char c= s.at(i);
				TamanioBitset indice= TAMANIO_STRING-1-i;
				retornar[indice]= c;
			}


			return retornar;

		}


	public:

		/*---------------------------------------------------------------------------------------------------------------------------------------*/

		/*
		 * DOCUMENTACION:
		 *
		 * a tener en cuenta:
		 * es una clase template que recibe un parametro tamanioBuffer, si dicho parametro es 0 -> se lanzara una exception en el constructor.
		 *
		 *el proposito original de esta clase es permitir la escritura de bits cuya cantidad no complete necesariamente el octeto.
		 *
		 * Metodos importantes:
		 *
		 * flush() -> borra todos los bits
		 *
		 * agregar_bits(string s) -> escribe en el buffer de bits a partir de un string
		 * con el formato "010010101..."
		 *
		 * agregar_bits(bool*,unsigned short) -> escribe en el buffer a partir de un arreglo
		 * de bool
		 *
		 * completa_octeto() -> verifica si en el buffer hay mas de 8 bits
		 *
		 * dump(char*) -> almacena los bytes COMPLETOS del buffer de bits en un arreglo
		 * de char.
		 *
		 * dump_y_completar(char*) -> almacena en un arreglo de char los bytes completos
		 * del buffer de bits y completa con 0s algun grupo de bits que no llene el octeto.
		 *
		 * get_indice_buffer() -> retorna en que posicion se ESCRIBIRA EL SIGUIENTE BIT
		 * . Viendolo de otra manera devuelve la cantidad de bits ya escritos
		 *
		 * pop_bit() -> remueve el primer bit del buffer
		 *
		 * pop_byte(&) -> remueve el primer byte del buffer de bits y lo
		 * retorna en una referencia &
		 *
		 * to_string() -> devuelve el contenido del buffer de bits como una cadena de
		 * caracteres de 0s y 1s
		 *
		 * set() -> setea todos los bits en 1
		 *
		 * reset() -> setea todos los bits en 0
		 */



		/*---------------------------------------------------------------------------------------------------------------------------------------*/


		class TamanioBufferIncorrecto:public exception{

		public:
			const char* what()const throw() {
				return "ExcepcionTamanioBufferIncorrecto";
			}
		};/*excepcion por tamanioBuffer inapropiado*/



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

		}/*limpia el buffer*/

		void reset(){
			buffer->reset();
		}/*setea todos los bits en 0*/
		void set(){
			buffer->set();
			this->bitActual= 0;
		}/*setea todos los bits en 1*/



		int agregar_bit(bool bit){

			if(bitActual>= tamanioBuffer)
				return RES_ERROR;

			buffer->set(bitActual,bit);
			bitActual++;

			return RES_OK;

		}/*agrega un bit al final del buffer. El tamanio del mismo aumenta*/
		int agregar_bit(int bit){

			if(bit)
				return agregar_bit(true);
			return agregar_bit(false);

		}
		int agregar_bits(string s){

			const TamanioBitset TAMANIO_STRING= s.length();
			if(TAMANIO_STRING + bitActual > tamanioBuffer )
				return RES_ERROR;

			for(TamanioBitset i=0;i<TAMANIO_STRING;i++){
				char c= s.at(i);
				if(c!= '1' && c!= '0')
					return RES_ERROR;

				bool b= (c=='1');
				this->agregar_bit(b);
			}

			return RES_OK;

		}
		int agregar_bits(bool* valores,TamanioBitset tamanioValores){

			if(tamanioValores+bitActual > tamanioBuffer)
				return RES_ERROR;
			if(!tamanioValores)
				return RES_ERROR;

			for(TamanioBitset i=0;i<tamanioValores;i++)
				this->agregar_bit(valores[i]);

			return RES_OK;

		}
		int agregar_bits(char valores){

			const TamanioBitset ESPACIO_DISPONIBLE= tamanioBuffer - bitActual;
			const unsigned short TAMANIO_BYTE_BITS= 8;
			if(ESPACIO_DISPONIBLE < TAMANIO_BYTE_BITS)
				return RES_ERROR;

			unsigned long const VALORES_LONG= (unsigned long)valores;
			bitset<8> bb(VALORES_LONG);

			for( short i= (TAMANIO_BYTE_BITS-1) ;i>=0; i--){

				bool b= bb[i];
				this->agregar_bit(b);

			}

			return RES_OK;

		}
		int get_bit(TamanioBitset posicion,bool& bit){

			if(posicion>= bitActual)
				return RES_ERROR;

			bit= (*buffer)[posicion];

//			string bufferString= to_string();
//			char c= bufferString.at(posicion);
//
//			bit= (c=='1');
			return RES_OK;

		}/*retorna el valor del bit en la posicion deseada*/


		string to_string(){

			if(bitActual== 0)
				return string("");

			string retornar;
			string bufferString= buffer->to_string();


			TamanioBitset i=tamanioBuffer-1;
			TamanioBitset indice= 0;

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
		TamanioBitset get_cantidad_bytes(){
			TamanioBitset resultado;
			resultado= (TamanioBitset)(bitActual/8);
			return resultado;
		}/*retorna la cantidad de bytes completos en buffer*/


		int to_char(){

			const unsigned short TAMANIO_CHAR= 8;
			if(tamanioBuffer != TAMANIO_CHAR)
				return RES_ERROR;
			if(bitActual<8)
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
		tamanioBuffer sea != a 8 bits o el valor de bitActual sea menor a 8 (no se han agregado mas de 8 bits aun)*/


		int get_byte(unsigned short numeroByte,Byte& retornar)throw(TamanioBufferIncorrecto){

			const unsigned short CANTIDAD_BYTES= get_cantidad_bytes();
			if(numeroByte>= CANTIDAD_BYTES)
				return RES_ERROR;
			if(tamanioBuffer< 8){
				TamanioBufferIncorrecto tbi;
				throw tbi;
			}


			BufferBits<8> bb;

			string bs= to_string();
			TamanioBitset bitNumeroByte= 8*numeroByte;

			for(unsigned short i=0;i<8;i++){

				TamanioBitset indice= bitNumeroByte+i;
				char c= bs.at(indice);
				bool b= c=='1';
				bb.agregar_bit(b);

			}

			retornar= (Byte)bb.to_char();

			return RES_OK;

		}/*retorna el byte en la posicion numeroByte. Los mismos se cuentan desde el 0*/
		int get_long(unsigned short numeroLong,unsigned long& retornar){


			if( bitActual< BITS_LONG )
				return RES_ERROR;
			const unsigned short CANTIDAD_LONGS= (unsigned short)( this->bitActual/BITS_LONG );
			if(numeroLong >= CANTIDAD_LONGS)
				return RES_ERROR;

			const TamanioBitset BIT_INSPECCIONAR= BITS_LONG*numeroLong;
			string bufferString= this->to_string();
			bitset<BITS_LONG> bitsetAuxiliar;

			for(unsigned short i=0;i<BITS_LONG;i++){

				TamanioBitset indice= BIT_INSPECCIONAR+i;
				bool valor= (bufferString.at(indice) == '1');
				bitsetAuxiliar.set( BITS_LONG - 1 - i , valor );

			}

			retornar= bitsetAuxiliar.to_ulong();

			return RES_OK;

		}


		int quitar_bit(TamanioBitset posicionBit){

			if(posicionBit>= bitActual)
				return RES_ERROR;

			string bufferString= this->to_string();
			bufferString.erase(posicionBit,1);

			this->flush();
			this->agregar_bits(bufferString);

			return RES_OK;

		}/*quita el bit en la posicion posicionBit y comprime los bits nuevamente*/
		int pop_bit(bool& bit){

			this->get_bit(0,bit);

			return quitar_bit(0);

		}/*quita el primer bit (el mas significativo)*/



		int quitar_bits(TamanioBitset posicionBitInicial,TamanioBitset cantidadBits){

			TamanioBitset indiceFinal= posicionBitInicial+cantidadBits;
			if(indiceFinal > bitActual)
				return RES_ERROR;

			for(TamanioBitset i=0;i<cantidadBits;i++)
				this->quitar_bit(posicionBitInicial);

			return RES_OK;

		}/*quita una cantidad de bits*/



		int quitar_byte(unsigned short numeroByte){

			TamanioBitset numeroBitByte= numeroByte*8;
			if( (numeroBitByte+8) > bitActual )
				return RES_ERROR;

			for(unsigned short i=0;i<8;i++)
				quitar_bit(numeroBitByte);

			return RES_OK;

		}/*remueve un byte completo*/
		int pop_byte(Byte& retornar){

			if(bitActual<8)
				return RES_ERROR;
			this->get_byte(0,retornar);

			return quitar_byte(0);


		}/*remueve el primer byte y lo retorna en la variable retornar*/

		bool completa_octeto(){

			return (bitActual>= 8);

		}/*retorna true si se han agregado mas de 8 bits al buffer*/


		int dump(char* escritura){

			if(!this->completa_octeto())
				return RES_ERROR;


			const unsigned short CANTIDAD_BYTES= this->get_cantidad_bytes();
			for(unsigned short i=0;i<CANTIDAD_BYTES;i++){

				Byte c;
				this->pop_byte(c);
				escritura[i]= c;

			}

			return CANTIDAD_BYTES;

		}/*Almacena en el buffer escritura los bytes completos en el buffer y los remueve del BufferBits.retorna la cantidad de bytes que fueron
		escritos en escritura. Retorna RES_ERROR en caso de error*/


		int dump_y_completar(char* escritura){

			int res= this->dump(escritura);
			if(res== RES_ERROR)
				res= 0;

			if( this->get_indice_buffer()==0 )
				return res;

			while( !this->completa_octeto() )
				this->agregar_bit(0);

			Byte c;
			this->pop_byte(c);
			escritura[res]= c;

			this->flush();

			return res+1;

		}/*almacena en el buffer escritura los bytes almacenados en el buffer y en caso de tener una cantidad de bits en el buffer tal que el mismo no
		completa el octeto -> se completa con ceros al final*/




		TamanioBitset get_espacio_disponible(){

			return (tamanioBuffer - bitActual);

		}

		bool esta_vacio(){

			return this->bitActual== 0;

		}


};






