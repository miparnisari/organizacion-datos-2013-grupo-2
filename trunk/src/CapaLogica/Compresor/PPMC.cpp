#include "PPMC.h"

PPMC::PPMC(unsigned int orden) : Compresor()
{
	orden_maximo = orden;
	mapa_ordenes = new TablaOrdenes(orden_maximo);
	comp_aritmetico = new Aritmetico();
}

PPMC::~PPMC()
{
	delete comp_aritmetico;
	delete mapa_ordenes;
}

void PPMC::_reset()
{
	delete comp_aritmetico;
	delete mapa_ordenes;
	mapa_ordenes = new TablaOrdenes(orden_maximo);
	comp_aritmetico = new Aritmetico();
}

void PPMC::_guardar_bits(char* bufferComprimido,
		Uint & indiceBufferComprimido,
		BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
		vector<bool> bits_a_emitir,
		bool es_ultimo)
{

	if (bufferComprimido== NULL)
		return;

	const Uint CANTIDAD_BITS_A_EMITIR= bits_a_emitir.size();
	if (CANTIDAD_BITS_A_EMITIR== 0)
		return;


	for (Uint i=0;i<CANTIDAD_BITS_A_EMITIR;i++){
		bool bit= bits_a_emitir.at(i);
		buffer_bits.agregar_bit(bit);
	}

	if (buffer_bits.completa_octeto())
	{
		char* puntero= bufferComprimido + indiceBufferComprimido;
		Uint const CANTIDAD_BYTES_ESCRIBIR = buffer_bits.get_cantidad_bytes();
		buffer_bits.dump( puntero );
		indiceBufferComprimido += CANTIDAD_BYTES_ESCRIBIR;
	}

	if (es_ultimo && ! buffer_bits.esta_vacio())
	{
		buffer_bits.dump_y_completar((char*) bufferComprimido+indiceBufferComprimido);
		Uint const CANTIDAD_BITS_BYTE = 8;
		indiceBufferComprimido += CANTIDAD_BITS_BYTE;
	}

}

void PPMC::_imprimir_emision(int orden, double probabilidad, Uint simbolo){

	stringstream res;
	res << "Emito ";

	if (64 <= simbolo && simbolo <= 127)
		res << (char)simbolo;
	else if (simbolo == VALOR_DEL_ESCAPE)
		res << "ESCAPE";
	else
		res << "(int) "<<simbolo;

	res << ", en orden " << orden << ", con probabilidad = " << probabilidad << ".";

	Logger::info("PPMC",res.str());
}

int PPMC::_comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;

	ModeloProbabilistico* modelo_actual = mapa_ordenes->devolver_modelo(orden,contexto_del_simbolo);

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;

	if (modelo_actual->get_frecuencia(simbolo) == 0)
	{
		resultado = RES_ESCAPE;

		_imprimir_emision(orden,modelo_actual->get_probabilidad(VALOR_DEL_ESCAPE),VALOR_DEL_ESCAPE);

		if (modelo_actual->get_frecuencia(VALOR_DEL_ESCAPE) == 1  && modelo_actual->calcular_total_frecuencias() == 1)
		{
			// Si el escape es el unico simbolo no se va a emitir nada asi que no hay que llamar a comprimir
		}
		else {
			// Si el escape NO es el unico simbolo, hay que comprimirlo
			a_emitir = comp_aritmetico->comprimir(VALOR_DEL_ESCAPE,cOverflow,cUnderflow);
		}
	}

	// El simbolo tenia frecuencia >=1.. hay que comprimirlo y no emitir escape
	else {
		resultado = RES_OK;

		ModeloProbabilistico* modelo_antes_de_comprimir = comp_aritmetico->get_modelo();

		_imprimir_emision(orden,modelo_actual->get_probabilidad(simbolo),simbolo);

		a_emitir = comp_aritmetico->comprimir(simbolo,cOverflow,cUnderflow);
		if (orden == -1)
			comp_aritmetico->set_modelo(modelo_antes_de_comprimir);
	}

	return resultado;
}

void PPMC::_comprimir_ultimo (std::vector<bool>& a_emitir)
{
	a_emitir.clear();
	a_emitir = comp_aritmetico->comprimir_ultimo_paso();
}


void PPMC::_comprimir_un_caracter(int& orden, Uint indiceSimbolo, const Uint simbolo , string& contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
		vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido,bool esUltimo)
{
//	mapa_ordenes->imprimir();

	if (orden == 0 || orden == -1)
		contexto = utilitarios::int_a_string(orden);

	string maximo_contexto_actual = contexto;

	if (esUltimo)
	{
		ModeloProbabilistico* modelo_actual = mapa_ordenes->devolver_modelo(orden,contexto);
		int resultado = RES_OK;
		//si no puedo emitir el ultimo en este contexto sigo de largo y comprimo el escape
		if (modelo_actual->get_frecuencia(simbolo) == 0)
			resultado = RES_ESCAPE;

		if (resultado != RES_ESCAPE)
		{
			_comprimir_ultimo (bits_a_emitir);

			_guardar_bits(bufferComprimido,indiceBufferComprimido,buffer_bits, bits_a_emitir, true);

			return;
		}
	}

	int res = _comprimir(simbolo, orden, contexto, bits_a_emitir);

	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir,false);
	bits_a_emitir.clear();
	mapa_ordenes->actualizar_modelo(orden,simbolo,contexto);

	while (res == RES_ESCAPE)
	{
		orden --;
		if (orden == 0 || orden == -1)
			contexto = utilitarios::int_a_string(orden);
		else
			contexto.erase(0,1);

		if (esUltimo)
		{
			ModeloProbabilistico* modelo_actual = mapa_ordenes->devolver_modelo(orden,contexto);
			int resultado = RES_OK;
			//si no puedo emitir el ultimo en este contexto sigo de largo y comprimo el escape
			if (modelo_actual->get_frecuencia(simbolo) == 0)
				resultado = RES_ESCAPE;
			//sino emito el piso y salgo
			if (resultado!= RES_ESCAPE)
			{
				_comprimir_ultimo (bits_a_emitir);

				_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir, true);
				return;
			}
		}

		res = _comprimir(simbolo, orden, contexto, bits_a_emitir);

		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir, false);
		bits_a_emitir.clear();
		mapa_ordenes->actualizar_modelo(orden,simbolo,contexto);
	}

	if (maximo_contexto_actual == "0")
		contexto = simbolo;
	else if (maximo_contexto_actual != "0" && maximo_contexto_actual.size() < (Uint)orden_maximo)
	{
		contexto= maximo_contexto_actual;
		contexto += simbolo;
	}
	else
	{
		contexto = maximo_contexto_actual.substr(1,maximo_contexto_actual.length());
		contexto += simbolo;
	}

	// Las siguientes cuentas hacen que en la proxima iteracion
	// se comience desde el orden apropiado
	// (en algun momento siempre va a empezar desde orden_maximo)

	orden = 1 + indiceSimbolo;
	if (orden > orden_maximo)
		orden = orden_maximo;

}

int PPMC::comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* bufferComprimido)
{
	Logger::info("PPMC", "Comenzando la compresión.");
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> buffer_bits;
	string contexto = "";
	int orden = 0;
	Uint indiceBufferComprimido = 0;
	vector<bool> bits_a_emitir;

	for (Uint indiceSimbolo = 0; indiceSimbolo < tamanioBuffer; indiceSimbolo++)
	{
		const unsigned char uc_simbolo = (unsigned char) buffer[indiceSimbolo];
		const Uint simbolo = (Uint) uc_simbolo;

		_comprimir_un_caracter(orden,indiceSimbolo,simbolo,contexto, buffer_bits,
				bits_a_emitir,bufferComprimido,indiceBufferComprimido,false);

		// Comprimir el ultimo caracter
		if (indiceSimbolo == tamanioBuffer -1)
			_comprimir_un_caracter(orden,indiceSimbolo, simbolo, contexto, buffer_bits,
						bits_a_emitir,bufferComprimido,indiceBufferComprimido,true);
	}


	Logger::info("PPMC", "Finalizó la compresión.");
	return indiceBufferComprimido;
}


int PPMC::_descomprimir(unsigned long valorSimbolo,string contextoActual,int numeroOrdenActual,Uint& simbolo,
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& bufferBits,Aritmetico& aritmeticoCopia){

	 ModeloProbabilistico* modeloActual= mapa_ordenes->devolver_modelo(numeroOrdenActual, contextoActual);

	if (modeloActual== NULL)
		return RES_ERROR;

	string contextoInferior;
	if (numeroOrdenActual == 0)
		contextoInferior = "-1";
	else
	if (numeroOrdenActual == 1)
		contextoInferior = "0";
	else{
		contextoInferior= contextoActual;
		contextoInferior.erase(0,1);
	}

	int numeroOrdenInferior= numeroOrdenActual-1;


	/*si escape es el unico caracter salto directamente al contexto inferior*/
	if (numeroOrdenActual != -1)
	{
		bool escapeEsUnicoCaracter= ( modeloActual->get_frecuencia(VALOR_DEL_ESCAPE)==1 &&
				modeloActual->calcular_total_frecuencias()== 1);
		if( escapeEsUnicoCaracter ){

			return _descomprimir(valorSimbolo,contextoInferior,numeroOrdenInferior,simbolo,bufferBits,aritmeticoCopia);

		}
	}

	/*si escape no es el unico caracter en el modelo actual intento descomprimir el valor del simbolo */

	/*creo una copia del aritmetico a usar a partir del modelo actual*/
	aritmeticoCopia.set_modelo( modeloActual );

	Byte cOverflow,cUnderflow;
	Uint simboloCopia= aritmeticoCopia.descomprimir( (Uint)valorSimbolo);
	aritmeticoCopia.comprimir(simboloCopia,cOverflow,cUnderflow);

	/*descarto los bits de overflow y underflow*/
	for(Uint i=0;i<cOverflow;i++)
		bufferBits.quitar_bit(0);
	for(Uint i=0;i<cUnderflow;i++)
		bufferBits.quitar_bit(1);


	bool simboloEsEscape= ( simboloCopia==VALOR_DEL_ESCAPE );

	/*si el simbolo que se descomprimio es el escape debo saltar a un contexto inferior*/
	if (simboloEsEscape){
		if( numeroOrdenActual == -1 )
		{
			Logger::info("PPMC","Se quiso descomprimir ESCAPE en orden -1.");
			return RES_ERROR;
		}

		bufferBits.get_primer_valor_numerico(PRECISION,valorSimbolo);

		return this->_descomprimir(valorSimbolo,contextoInferior,numeroOrdenInferior,simbolo,bufferBits,aritmeticoCopia);
	}


	/*si el simbolo descompreso NO ES ESCAPE ...*/

	simbolo= simboloCopia;
	return RES_OK;


}


int PPMC::descomprimir_todo
	(char* bufferComprimido,
	unsigned int tamanioBufferComprimido,
	char* bufferDescomprimido,
	unsigned int cantidadCaracteresOriginal){

	_reset();

	Logger::info("PPMC", "Comenzando descompresión.");

	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> bufferBits;
	string nombreContexto= "0";
	int numeroOrden= 0;
	Uint indiceBufferComprimido= 0;

	const Uint TAMANIO_BUFFER_BITS_BYTES= (Uint)(TAMANIO_BUFFER_BITS_DEFAULT/8);

	for(Uint i=0; i<TAMANIO_BUFFER_BITS_BYTES && i<tamanioBufferComprimido ; i++ ){

		Byte byte= (Byte)bufferComprimido[i];
		bufferBits.agregar_bits(byte);
		indiceBufferComprimido++;

	}


	for( Uint caracterActual= 0; caracterActual < cantidadCaracteresOriginal ; caracterActual++ ){

		Uint simbolo;
		vector<bool> bitsEmitir;

		unsigned long valor;
		bufferBits.get_primer_valor_numerico(PRECISION,valor);

		Aritmetico aritmeticoCopia( TAMANIO_ALFABETO );
		Intervalo* intervaloCopia= this->comp_aritmetico->get_intervalo();
		aritmeticoCopia.set_intervalo( intervaloCopia );

		/*descomprimo un caracter. Dentro del metodo se descartan bits del buffer de bits*/
		int resultadoDescomprimir= this->_descomprimir(valor,nombreContexto,numeroOrden,simbolo,bufferBits,aritmeticoCopia);
		if (resultadoDescomprimir== RES_ERROR)
			return RES_ERROR;

		bufferDescomprimido[caracterActual]= (char)simbolo;

		this->_comprimir_un_caracter(numeroOrden,caracterActual,simbolo,nombreContexto,bufferBits,bitsEmitir,
				NULL,indiceBufferComprimido,false);

		/*intento rellenar el buffer de bits*/
		TamanioBitset espacioDisponibleBufferBitsBytes=(TamanioBitset)( bufferBits.get_espacio_disponible() /8 );

		if(indiceBufferComprimido < tamanioBufferComprimido)
		for( TamanioBitset i=0; i<espacioDisponibleBufferBitsBytes ;i++ ){
			Byte byte= (Byte)bufferComprimido[indiceBufferComprimido];
			bufferBits.agregar_bits( byte );
			indiceBufferComprimido++;
		}
	}

	Logger::info("PPMC", "Finalizó la descompresión.");

	return RES_OK;
}

