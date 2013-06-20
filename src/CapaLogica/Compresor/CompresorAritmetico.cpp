/*
 * CompresorAritmetico.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#include "CompresorAritmetico.h"

CompresorAritmetico::CompresorAritmetico()
{
	modelo.inicializar_frecuencias_en_1();
	byteActual = '0';
}

CompresorAritmetico::~CompresorAritmetico()
{

}

std::vector<bool> CompresorAritmetico::comprimir(const char simbolo)
{
	intervalo.calcular_rango();

	double low_count_simbolo = modelo.calcular_low_count(simbolo);
	double high_count_simbolo = modelo.calcular_high_count(simbolo);

	/*
	 * Nuevo Piso = 0 + 256 * 3 / 6 = 128
	 * Nuevo Techo = 0 + 256 * 5 / 6 - 1 = 212
	 */
	intervalo.actualizar_piso_techo(low_count_simbolo, high_count_simbolo);

	/*
	 * Emision: 1
	 * Contador de underflow: 0
	 * Modifica piso y techo.
	 */
	std::vector<bool> bits_a_emitir = intervalo.normalizar();

	modelo.incrementar_frecuencia(simbolo);

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

//			// Ciclo que recorre los bits que tengo que emitir
//			for (unsigned int j = 0; j < bits_caracter_actual.size(); j++)
//			{
//				bitsAemitir.push_back(bits_caracter_actual[j]);
//				if (bitsAemitir.size() == 8)
//				{
//					// byte_a_emitir = bits_a_emitir
//					// comprimido += byte_a_emitir
//					tamanioComprimido ++;
//					bitsAemitir.clear();
//				}
//			}


			const TamanioBitset TAMANIO_BITS_CARACTER_ACTUAL= bits_caracter_actual.size();
			for( TamanioBitset j=0;i<TAMANIO_BITS_CARACTER_ACTUAL;j++ ){

				bool bit= bits_caracter_actual.at(j);

				if( bufferBits.agregar_bit(bit)== RES_ERROR ){
					char bufferTemporal[TAMANIO_BUFFER_BITS_BYTES];
					bufferBits.dump( bufferTemporal );
					stream.write( bufferTemporal,TAMANIO_BUFFER_BITS_BYTES );
					bufferBits.agregar_bit(bit);

					tamanioComprimido+= TAMANIO_BUFFER_BITS_BYTES;
				}


			}//for


		}

		if(bufferBits.get_indice_buffer()== 0)
			return tamanioComprimido;

		char bufferAuxiliar[TAMANIO_BUFFER_BITS_BYTES];
		TamanioBitset tamanioUltimaEscritura= bufferBits.dump_y_completar(bufferAuxiliar);
		stream.write( bufferAuxiliar,tamanioUltimaEscritura );

		stream.seekg(0,ios::beg);
		stream.read( bufferCompresion,tamanioComprimido );


	return tamanioComprimido;

}
