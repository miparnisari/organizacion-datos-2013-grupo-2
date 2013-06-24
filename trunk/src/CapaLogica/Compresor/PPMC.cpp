#include "PPMC.h"


PPMC::PPMC(unsigned short orden) : Compresor()
{
	orden_maximo = orden;
	comp_aritmetico = new Aritmetico();

	_inicializar_orden_menosuno();
	_inicializar_orden_cero();

	// ORDEN 1 a "orden"
	for (unsigned short i = 1; i < orden+1; i++)
	{
		Orden* contexto_vacio = new Orden();
		mapa_ordenes.insert(pair<int,Orden*> (i,contexto_vacio));
	}

	archivoSalida= new ofstream(ARCHIVO_EMISION);

}

void PPMC::_inicializar_orden_menosuno()
{
	ModeloProbabilistico* mod_menos_uno = new ModeloProbabilistico(TAMANIO_ALFABETO-1);
	mod_menos_uno->inicializar_frecuencias_en_1();

	Orden* contexto_menos_uno = new Orden();
	contexto_menos_uno->agregar_modelo("-1",mod_menos_uno);
	mapa_ordenes.insert(pair<int,Orden*> (-1,contexto_menos_uno));
}

void PPMC::_inicializar_orden_cero()
{
	ModeloProbabilistico* mod_cero = new ModeloProbabilistico(TAMANIO_ALFABETO);
	vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	mod_cero->inicializar_frecuencias_en_1(vector);

	Orden* ctx_cero = new Orden();
	ctx_cero->agregar_modelo("0",mod_cero);
	mapa_ordenes.insert(pair<int,Orden*> (0,ctx_cero));
}


PPMC::~PPMC()
{
	archivoSalida->seekp(0,ios::end);
	archivoSalida->close();
	delete comp_aritmetico;
	delete archivoSalida;
}


void PPMC::_crear_modelo_vacio (int orden, string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	Orden* contexto = mapa_ordenes[orden];
	contexto->agregar_modelo(nombre_modelo,modelo);
	mapa_ordenes[orden] = contexto;
}

void PPMC::_guardar_bits(char* bufferComprimido,
		Uint & indiceBufferComprimido,
		BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
		vector<bool> bits_a_emitir)
{

	if(bufferComprimido== NULL)
		return;

	const Uint CANTIDAD_BITS_A_EMITIR= bits_a_emitir.size();
	if (CANTIDAD_BITS_A_EMITIR== 0)
		return;


	for (Uint i=0;i<CANTIDAD_BITS_A_EMITIR;i++){
		bool bit= bits_a_emitir.at(i);
		buffer_bits.agregar_bit(bit);//fixme puede que aca se llene el buffer y de error, supongo que en ese caso hay que hacer dump
	}

	if (buffer_bits.completa_octeto())
	{
		char* puntero= bufferComprimido + indiceBufferComprimido;
		Uint const CANTIDAD_BYTES_ESCRIBIR = buffer_bits.get_cantidad_bytes();
		buffer_bits.dump( puntero );
		indiceBufferComprimido += CANTIDAD_BYTES_ESCRIBIR;
	}

}

void PPMC::_comprimir_ultimo_paso(Uint simbolo, int orden, string contexto,
		char* bufferComprimido, Uint indiceBufferComprimido, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits)
{
	ModeloProbabilistico* modelo_actual = NULL;
	int res = mapa_ordenes[orden]->devolver_modelo(contexto,&modelo_actual);
	if (res == RES_ERROR)
	{
		_crear_modelo_vacio(orden, contexto);
		mapa_ordenes[orden]->devolver_modelo(contexto,&modelo_actual);
	}

	comp_aritmetico->set_modelo (modelo_actual);

	vector<bool> bits_a_emitir = comp_aritmetico->comprimir_ultimo_paso();
	_guardar_bits(bufferComprimido,indiceBufferComprimido,buffer_bits,bits_a_emitir);

}

void PPMC::_imprimir_todos_ordenes()
{
	for (int orden = -1; orden < orden_maximo + 1; orden++)
	{
		(*archivoSalida) << "---------------- ORDEN " << orden << " ----------------" << endl;
		map<string, ModeloProbabilistico*> mapa_modelos = mapa_ordenes[orden]->get_mapa_modelos();
		map<string, ModeloProbabilistico*>::iterator iterador = mapa_modelos.begin();
		while (iterador != mapa_modelos.end())
		{
			(*archivoSalida) << "~~~~~~ CTX " << (*iterador).first << " ~~~~~~" <<endl;
			ModeloProbabilistico* unModelo = (*iterador).second;
			(*archivoSalida)<<'\t';
			unModelo->imprimir( archivoSalida );
			iterador ++;
		}
	}

	(*archivoSalida)<<'\n';

}

void PPMC::_actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo)
{
	// Si el orden es -1 no se debe actualizar nada
	if (orden == -1)
		return;

	ModeloProbabilistico* modelo;
	mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo);

	if ((modelo->get_frecuencia(VALOR_DEL_ESCAPE)==1) && (modelo->calcular_total_frecuencias()==1)) {
		// Si solo existia el escape, solo aumento la frecuencia del simbolo
	}
	else if (modelo->get_frecuencia(simbolo) >= 1)
	{
		// Si el simbolo tenia frecuencia >= 1, solo aumento la frecuencia del simbolo
	}
	else{
//		cout << "INCREMENTAR FRECUENCIA DE " << VALOR_DEL_ESCAPE << " EN CTX " << contexto_del_simbolo << " EN ORDEN " << orden << endl;
		mapa_ordenes[orden]->incrementar_frecuencia(VALOR_DEL_ESCAPE,contexto_del_simbolo);
	}
//	cout << "INCREMENTAR FRECUENCIA DE " << (char)simbolo << " EN CTX " << contexto_del_simbolo << " EN ORDEN " << orden << endl;
	mapa_ordenes[orden]->incrementar_frecuencia(simbolo,contexto_del_simbolo);


	if (orden != orden_maximo)
	{
		// En el contexto superior, creamos el contexto "contexto + simbolo"
		// solo con el ESCAPE

		string ctx = "";
		if ((orden != -1) && (orden != 0))
			ctx += contexto_del_simbolo;
		ctx += simbolo;

		ModeloProbabilistico* modelo_contexto_superior;
		int res = mapa_ordenes[orden+1]->devolver_modelo(ctx,&modelo_contexto_superior);

		if (res == RES_ERROR)
		{
//			cout << "VOY A CREAR CONTEXTO " << ctx << " EN ORDEN " << orden +1 << endl;
			_crear_modelo_vacio(orden+1,ctx);
		}
	}

}


void PPMC::_imprimir_estado(int orden,double probabilidad,Uint simbolo){


	stringstream streamEmision;
	streamEmision<<"emito ";

	if(64<= simbolo && simbolo <= 127)
		streamEmision<<(char)simbolo;
	else
		streamEmision<<simbolo;

	streamEmision<<", en orden ";
	streamEmision<<orden;
	streamEmision<<" ,con probabilidad = ";
	streamEmision<<probabilidad;
	streamEmision<<".";
	streamEmision<<'\n';
//	streamEmision.seekg(0,ios::beg);
	string stringEmision= streamEmision.str();


	cout<<stringEmision;
	this->archivoSalida->write( stringEmision.c_str() , stringEmision.length() );

}


int PPMC::comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;
	ModeloProbabilistico* modelo_actual = NULL;

	mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo_actual);

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;

	if (modelo_actual->get_frecuencia(simbolo) == 0) {
		resultado = RES_ESCAPE;

//		_imprimir_estado(orden,modelo_actual->get_probabilidad(VALOR_DEL_ESCAPE),VALOR_DEL_ESCAPE);

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

		_imprimir_todos_ordenes();

//		_imprimir_estado(orden,modelo_actual->get_probabilidad(simbolo),simbolo);

		a_emitir = comp_aritmetico->comprimir(simbolo,cOverflow,cUnderflow);
		if (orden == -1)
			comp_aritmetico->set_modelo(modelo_antes_de_comprimir);
	}

	return resultado;
}

void PPMC::_comprimir_ultimo (std::vector<bool>& a_emitir)
{
	a_emitir=comp_aritmetico->comprimir_ultimo_paso();
}

void PPMC::_emitir_completando_octeto(char* bufferComprimido,
		Uint & indiceBufferComprimido,
		BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
		vector<bool> bits_a_emitir)
{
	if( bufferComprimido== NULL )
		return;

	const Uint CANTIDAD_BITS_A_EMITIR= bits_a_emitir.size();

	for (Uint i=0;i<CANTIDAD_BITS_A_EMITIR;i++){
		bool bit= bits_a_emitir.at(i);
		buffer_bits.agregar_bit(bit);//fixme puede que aca se llene el buffer y de error, supongo que en ese caso hay que hacer dump
	}

	if (buffer_bits.completa_octeto())
	{
		char* puntero= bufferComprimido + indiceBufferComprimido;
		Uint const CANTIDAD_BYTES_ESCRIBIR = buffer_bits.get_cantidad_bytes();
		buffer_bits.dump( puntero );
		indiceBufferComprimido += CANTIDAD_BYTES_ESCRIBIR;
	}

	if(!buffer_bits.esta_vacio())
	{
		buffer_bits.dump_y_completar((char*) bufferComprimido+indiceBufferComprimido);
		Uint const CANTIDAD_BITS_BYTE = 8;
		indiceBufferComprimido += CANTIDAD_BITS_BYTE;
	}
}

void PPMC::comprimir_un_caracter(int& orden, Uint indiceSimbolo, const Uint simbolo , string& contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
		vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido,bool esUltimo)
{
	if (orden == 0 || orden == -1)
		contexto = utilitarios::int_a_string(orden);

	string maximo_contexto_actual = contexto;

	if (esUltimo)
	{
		ModeloProbabilistico* modelo_actual = NULL;
		mapa_ordenes[orden]->devolver_modelo(contexto,&modelo_actual);
		int resultado;
		//si no puedo emitir el ultimo en este contexto sigo de largo y comprimo el escape
		if (modelo_actual->get_frecuencia(simbolo) == 0)
			resultado = RES_ESCAPE;

		if (resultado!= RES_ESCAPE)
		{
			_comprimir_ultimo (bits_a_emitir);

			_emitir_completando_octeto(bufferComprimido, indiceBufferComprimido,
									   buffer_bits, bits_a_emitir);
			return;
		}
	}

	int res = comprimir(simbolo, orden, contexto, bits_a_emitir);

	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);
	bits_a_emitir.clear();
	_actualizar_contexto(orden, simbolo, contexto);

	while (res == RES_ESCAPE)
	{
		orden --;
		if (orden == 0 || orden == -1)
			contexto = utilitarios::int_a_string(orden);
		else
			contexto.erase(0,1);

		if (esUltimo)
		{
			ModeloProbabilistico* modelo_actual = NULL;
			mapa_ordenes[orden]->devolver_modelo(contexto,&modelo_actual);
			int resultado;
			//si no puedo emitir el ultimo en este contexto sigo de largo y comprimo el escape
			if (modelo_actual->get_frecuencia(simbolo) == 0)
				resultado = RES_ESCAPE;
			//sino emito el piso y salgo
			if (resultado!= RES_ESCAPE)
			{
				_comprimir_ultimo (bits_a_emitir);

				_emitir_completando_octeto(bufferComprimido, indiceBufferComprimido,
										   buffer_bits, bits_a_emitir);
				return;
			}
		}

		res = comprimir(simbolo, orden, contexto, bits_a_emitir);

		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);
		bits_a_emitir.clear();
		_actualizar_contexto(orden, simbolo, contexto);
	}

	if (maximo_contexto_actual == "0")
		contexto = simbolo;
	else if (maximo_contexto_actual != "0" && maximo_contexto_actual.size() < orden_maximo)
	{
		contexto= maximo_contexto_actual;
		contexto += simbolo;
	}
	else
	{
		contexto = maximo_contexto_actual.substr(1,maximo_contexto_actual.length());
		contexto += simbolo;
	}

	orden += 2 + indiceSimbolo;
	if (orden > orden_maximo)
		orden = orden_maximo;

}

int PPMC::comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* bufferComprimido)
{
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> buffer_bits;
	string contexto = "";
	int orden = 0;
	Uint indiceBufferComprimido = 0;
	vector<bool> bits_a_emitir;

	Uint indiceSimbolo;

	for (indiceSimbolo = 0; indiceSimbolo < tamanioBuffer - 1; indiceSimbolo++)
	{
		const unsigned char uc_simbolo = (unsigned char) buffer[indiceSimbolo];
		const Uint simbolo = (Uint) uc_simbolo;
//		cout << "SIMBOLO A COMPRIMIR = " << simbolo << endl;

		comprimir_un_caracter(orden,indiceSimbolo,simbolo,contexto, buffer_bits,
				bits_a_emitir,bufferComprimido,indiceBufferComprimido,false);
	}

	comprimir_un_caracter(orden,indiceSimbolo, buffer[indiceSimbolo],contexto, buffer_bits,
			bits_a_emitir,bufferComprimido,indiceBufferComprimido,true);

	return indiceBufferComprimido;
}


int PPMC::descomprimir(unsigned long valorSimbolo,string contextoActual,int numeroOrdenActual,Uint& simbolo,
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& bufferBits,Aritmetico& aritmeticoCopia){

	Orden* ordenActual= this->mapa_ordenes[numeroOrdenActual];
	ModeloProbabilistico* modeloActual;
	int resultadoDevolverModelo= ordenActual->devolver_modelo(contextoActual, &modeloActual);

	if(resultadoDevolverModelo== RES_ERROR)
		return RES_ERROR;

	bool escapeEsUnicoCaracter= ( modeloActual->get_frecuencia(VALOR_DEL_ESCAPE)==1 &&
			modeloActual->calcular_total_frecuencias()== 1);


	string contextoInferior;
	if( numeroOrdenActual==0 )
		contextoInferior= "-1";
	else
	if(numeroOrdenActual== 1)
		contextoInferior= "0";
	else{

		contextoInferior= contextoActual;
		contextoInferior.erase(0,1);


	}
	IMPRIMIR_MY_VARIABLE(contextoActual);
	IMPRIMIR_MY_VARIABLE(contextoInferior);



	int numeroOrdenInferior= numeroOrdenActual-1;


	/*si escape es el unico caracter salto directamente al contexto inferior*/
	if( escapeEsUnicoCaracter ){

		return descomprimir(valorSimbolo,contextoInferior,numeroOrdenInferior,simbolo,bufferBits,aritmeticoCopia);

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
	if(simboloEsEscape){
		/*si el simbolo es escape pero me encuentro en el contexto -1 ocurrio un ERROR FATAL*/
		if( numeroOrdenActual == -1 )
			return RES_ERROR;

		 bufferBits.get_primer_valor_numerico(PRECISION,valorSimbolo);

		return this->descomprimir(valorSimbolo,contextoInferior,numeroOrdenInferior,simbolo,bufferBits,aritmeticoCopia);
	}


	/*si el simbolo descompreso NO ES ESCAPE ...*/

	simbolo= simboloCopia;
	return RES_OK;


}


int PPMC::descomprimir_todo
	(char* bufferComprimido,
	unsigned int tamanioBufferComprimido,
	char* bufferDescomprimido,
	unsigned int precision,
	unsigned int cantidadCaracteresOriginal){

	/*considero que el compresor esta propiamente inicializado y no ha sido usado para comprimir antes de
	 * descomprimir*/

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

		IMPRIMIR_MY_VARIABLE(bufferBits.to_string());

		Uint simbolo;
		vector<bool> bitsEmitir;

		unsigned long valor;
		bufferBits.get_primer_valor_numerico(PRECISION,valor);

		Aritmetico aritmeticoCopia( TAMANIO_ALFABETO );
		Intervalo* intervaloCopia= this->comp_aritmetico->get_intervalo();
		aritmeticoCopia.set_intervalo( intervaloCopia );

		/*descomprimo un caracter. Dentro del metodo se descartan bits del buffer de bits*/
		int resultadoDescomprimir= this->descomprimir(valor,nombreContexto,numeroOrden,simbolo,bufferBits,aritmeticoCopia);
		if(resultadoDescomprimir== RES_ERROR)
			return RES_ERROR;
		bufferDescomprimido[caracterActual]= (char)simbolo;

		this->comprimir_un_caracter(numeroOrden,caracterActual,simbolo,nombreContexto,bufferBits,bitsEmitir,
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

	return RES_OK;

}

