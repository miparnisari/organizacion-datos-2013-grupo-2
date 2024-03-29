/*
 * CompresorAritmetico.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#include "Aritmetico.h"

Aritmetico::Aritmetico(Uint tamanioAlfabeto)
{
	modelo= new ModeloProbabilistico(tamanioAlfabeto);
	intervalo= new Intervalo();
	modelo->inicializar_frecuencias_en_1();
}

Aritmetico :: Aritmetico (const ModeloProbabilistico & unModelo)
{
	modelo = new ModeloProbabilistico(unModelo);
	intervalo = new Intervalo();
}

Aritmetico::~Aritmetico()
{
	delete modelo;
	delete intervalo;
}

void Aritmetico::_resetear()
{
	delete intervalo;
	modelo->resetear();
	intervalo= new Intervalo();
}

void Aritmetico::inicializar_frecuencias_en_1(vector<unsigned short>& v){
	modelo->inicializar_frecuencias_en_1(v);
}

Uint Aritmetico::descomprimir(Uint valor)
{
	const unsigned long long piso = intervalo->get_piso().to_ulong();
	const Ulonglong rango = intervalo->get_rango();
	const Uint tam_alfabeto = modelo->get_tamanio_alfabeto();


	for (Uint i = 0; i < tam_alfabeto; i++)
	{
		double low_caracter =  modelo->calcular_low_count(i);
		double high_caracter = modelo->calcular_high_count(i);
		Uint piso_caracter = floor (piso + rango * low_caracter);
		Uint techo_caracter = floor (piso + rango * high_caracter);
		if (techo_caracter != piso_caracter)
			techo_caracter -= 1;

		//fixme ... revisar. Condicion anterior no incluia el '!='
		if (piso_caracter <= valor && valor <= techo_caracter && (piso_caracter!= techo_caracter) )
		{
			return i;
		}
	}

	return tam_alfabeto;
}


std::vector<bool> Aritmetico::comprimir_ultimo_paso(){

	BitsPiso piso= intervalo->get_piso();
	std::vector<bool> retornar;

	Byte cOverflow,cUnderflow;
	intervalo->normalizar(cOverflow,cUnderflow);

	retornar.push_back( piso[PRECISION-1] );
	bool negado= !piso[PRECISION-1];
	const Uint CANTIDAD_UNDERFLOW= intervalo->get_contador_underflow();

	for(Uint i=0;i<CANTIDAD_UNDERFLOW;i++)
		retornar.push_back(negado);

	for( int i=PRECISION-2 ; i>=0 ; i-- ){
		retornar.push_back( piso[i] );
	}

	return retornar;

}

ModeloProbabilistico* Aritmetico::get_modelo()
{
	return modelo;
}

std::vector<bool> Aritmetico::comprimir(const Uint simbolo,Byte& cOverflow,Byte& cUnderflow)
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

	std::vector<bool> bits_a_emitir = intervalo->normalizar(cOverflow,cUnderflow);

	modelo->incrementar_frecuencia(simbolo);
	intervalo->calcular_rango();

	return bits_a_emitir;
}

int Aritmetico::comprimir_todo
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
		unsigned char caracter_a_comprimir = *(buffer_a_comprimir+i);
		Byte cOverflow,cUnderflow;
		std::vector<bool> bits_caracter_actual = comprimir(caracter_a_comprimir,cOverflow,cUnderflow);

		// FIXME refactorizar
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

	std::vector<bool> bits_caracter_actual = comprimir_ultimo_paso();

	const TamanioBitset TAMANIO_BITS_CARACTER_ACTUAL= bits_caracter_actual.size();
	for( TamanioBitset j=0;j<TAMANIO_BITS_CARACTER_ACTUAL;j++ )
	{

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


/*fixme arreglar el hecho de trabajar con una precision que no es multiplo de 8*/
int Aritmetico::descomprimir_todo(char* bufferComprimido,
		unsigned int tamanioBufferComprimido,
		char* bufferDescomprimido,
		unsigned int cantidadCaracteresOriginal)
{

	_resetear();

	unsigned int indiceBufferComprimido= 0;

	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> bufferBitsDescompresion;
	const unsigned int BYTES_BUFFER_BITS= (unsigned int)(TAMANIO_BUFFER_BITS_DEFAULT / 8);

//	if( BYTES_PRECISION < tamanioBufferComprimido )
//		return RES_ERROR;

	/*cargo inicialmente el buffer de bits llenandolo*/
	for(unsigned int i=0;i<BYTES_BUFFER_BITS && i<tamanioBufferComprimido ;i++){
		unsigned char c= bufferComprimido[i];
		bufferBitsDescompresion.agregar_bits(c);
		indiceBufferComprimido++;
	}

	for( unsigned int indiceCaracterActual= 0; indiceCaracterActual<cantidadCaracteresOriginal ; indiceCaracterActual++ ){

		unsigned long valorSimboloActual;
		bufferBitsDescompresion.get_primer_valor_numerico(PRECISION,valorSimboloActual);

		Uint simboloActual= descomprimir((Uint)valorSimboloActual);
		/*recupero un simbolo*/

		Byte cantidadOverflow,cantidadUnderflow;
		this->comprimir( simboloActual, cantidadOverflow , cantidadUnderflow );
		bufferDescomprimido[indiceCaracterActual] = simboloActual;
		/*guardo el simbolo recuperado*/

		for(Byte overflow=0;overflow<cantidadOverflow;overflow++)
			bufferBitsDescompresion.quitar_bit(0);

		for(Byte underflow=0;underflow<cantidadUnderflow;underflow++)
			bufferBitsDescompresion.quitar_bit(1);
		/*descarto bits a partir de la cantidad de overflow y underflow resueltos*/


		if(indiceBufferComprimido<tamanioBufferComprimido && (cantidadOverflow || cantidadUnderflow) ){

			unsigned short cantidadBytesLeer= (unsigned short)( (cantidadOverflow+cantidadUnderflow)/8 );
			cantidadBytesLeer++;

			for( unsigned short i=0;i<cantidadBytesLeer;i++ )
				if( bufferBitsDescompresion.agregar_bits( (Byte)bufferComprimido[indiceBufferComprimido] )!=RES_ERROR )
					indiceBufferComprimido++;

		}
		/*intento agregar un byte mas al buffer de bits*/

	}

	return RES_OK;
}


int Aritmetico::set_modelo(ModeloProbabilistico* mp){

	if(mp== NULL)
		return RES_ERROR;

	(*modelo)= (*mp);

	return RES_OK;

}

Intervalo* Aritmetico::get_intervalo(){

	return this->intervalo;

}

void Aritmetico::set_intervalo(Intervalo* otroIntervalo){

	if(otroIntervalo== NULL)
		return;

	BitsPiso piso= otroIntervalo->get_piso();
	BitsTecho techo= otroIntervalo->get_techo();
	string stringPiso= piso.to_string();
	string stringTecho= techo.to_string();

	this->intervalo->set_piso( stringPiso );
	this->intervalo->set_techo( stringTecho );

	this->intervalo->calcular_rango();
}

