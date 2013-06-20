/*
 * CompresorAritmetico.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#include "CompresorAritmetico.h"

CompresorAritmetico::CompresorAritmetico(unsigned int tamanioAlfabeto)
{
	modelo= new ModeloProbabilistico(tamanioAlfabeto);
	intervalo= new Intervalo();
	modelo->inicializar_frecuencias_en_1();
	byteActual = '0';
}

CompresorAritmetico :: CompresorAritmetico (ModeloProbabilistico* unModelo)
{
	modelo = unModelo;
	intervalo= new Intervalo();
	byteActual = '0';
}

CompresorAritmetico::~CompresorAritmetico()
{
	delete modelo;
	delete intervalo;
}

void CompresorAritmetico::inicializar_frecuencias_en_1(vector<unsigned short>& v){
	modelo->inicializar_frecuencias_en_1(v);
}


std::vector<bool> CompresorAritmetico::_comprimir_ultimo_paso(){

	BitsPiso piso= intervalo->get_piso();
	std::vector<bool> retornar;

	retornar.push_back( piso[PRECISION-1] );
	bool negado= !piso[PRECISION-1];
	const Uint CANTIDAD_UNDERFLOW= intervalo->get_contador_underflow();
	IMPRIMIR_MY_VARIABLE(CANTIDAD_UNDERFLOW);
	for(Uint i=0;i<CANTIDAD_UNDERFLOW;i++)
		retornar.push_back(negado);

	for( int i=PRECISION-2 ; i>=0 ; i-- ){
		retornar.push_back( piso[i] );
	}

	return retornar;


}



std::vector<bool> CompresorAritmetico::comprimir(const char simbolo)
{
	intervalo->calcular_rango();

	double low_count_simbolo = (double)modelo->calcular_low_count(simbolo);
	double high_count_simbolo = (double)modelo->calcular_high_count(simbolo);

	/*
	 * Nuevo Piso = 0 + 256 * 3 / 6 = 128
	 * Nuevo Techo = 0 + 256 * 5 / 6 - 1 = 212
	 */
	intervalo->actualizar_piso_techo(low_count_simbolo, high_count_simbolo);

	/*
	 * Emision: 1
	 * Contador de underflow: 0
	 * Modifica piso y techo.
	 */

	Byte cOverflow,cUnderflow;
	std::vector<bool> bits_a_emitir = intervalo->normalizar(cOverflow,cUnderflow);

	modelo->incrementar_frecuencia(simbolo);

	return bits_a_emitir;
}

int CompresorAritmetico::comprimir_todo
	(const char* buffer_a_comprimir,
	const unsigned int tamanio,
	char* bufferCompresion)
{

	BufferBits<TAMANIO_BUFFER_BITS_BITS> bufferBits;
	stringstream stream;


	int tamanioComprimido = 0;
	std::vector<bool> bitsAemitir;
	//char byte_a_emitir = '0';




		// Ciclo que recorre el buffer de entrada
		for (unsigned int i = 0; i < tamanio; i++)
		{
			std::vector<bool> bits_caracter_actual = comprimir(*(buffer_a_comprimir+i));

			const TamanioBitset TAMANIO_BITS_CARACTER_ACTUAL= bits_caracter_actual.size();
			for( TamanioBitset j=0;j<TAMANIO_BITS_CARACTER_ACTUAL;j++ ){

				bool bit= bits_caracter_actual.at(j);

				if( bufferBits.agregar_bit(bit)== RES_ERROR ){
					char bufferTemporal[TAMANIO_BUFFER_BITS_BYTES];
					bufferBits.dump( bufferTemporal );
					stream.write( bufferTemporal,TAMANIO_BUFFER_BITS_BYTES );
					bufferBits.agregar_bit(bit);

					tamanioComprimido+= TAMANIO_BUFFER_BITS_BYTES;
				}


			}//for j<TAMANIO_BITS_CARACTER_ACTUAL

		}//for i<tamanio

		std::vector<bool> bits_caracter_actual = _comprimir_ultimo_paso();

		const TamanioBitset TAMANIO_BITS_CARACTER_ACTUAL= bits_caracter_actual.size();
		for( TamanioBitset j=0;j<TAMANIO_BITS_CARACTER_ACTUAL;j++ ){

			bool bit= bits_caracter_actual.at(j);

			if( bufferBits.agregar_bit(bit)== RES_ERROR ){
				char bufferTemporal[TAMANIO_BUFFER_BITS_BYTES];
				bufferBits.dump( bufferTemporal );
				stream.write( bufferTemporal,TAMANIO_BUFFER_BITS_BYTES );
				bufferBits.agregar_bit(bit);

				tamanioComprimido+= TAMANIO_BUFFER_BITS_BYTES;
			}


		}//for j<TAMANIO_BITS_CARACTER_ACTUAL



		if(bufferBits.get_indice_buffer()!=0){

			char bufferAuxiliar[TAMANIO_BUFFER_BITS_BYTES];
			TamanioBitset tamanioUltimaEscritura= bufferBits.dump_y_completar(bufferAuxiliar);
			stream.write( bufferAuxiliar,tamanioUltimaEscritura );
			tamanioComprimido+= tamanioUltimaEscritura;

		}

		stream.seekg(0,ios::beg);
		stream.read( bufferCompresion,tamanioComprimido );


	return tamanioComprimido;

}
