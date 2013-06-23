#include "PPMC.h"


PPMC::PPMC(unsigned short orden) : Compresor()
{
	orden_maximo = orden;
	comp_aritmetico = new Aritmetico();

	// ORDEN -1
	ModeloProbabilistico* mod_menos_uno = new ModeloProbabilistico(TAMANIO_ALFABETO-1);
	mod_menos_uno->inicializar_frecuencias_en_1();

	Contextos* contexto_menos_uno = new Contextos();
	contexto_menos_uno->agregar_modelo("-1",mod_menos_uno);
	mapa_ordenes.insert(pair<int,Contextos*> (-1,contexto_menos_uno));

	// ORDEN 0
	ModeloProbabilistico* mod_cero = new ModeloProbabilistico(TAMANIO_ALFABETO);
	vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	mod_cero->inicializar_frecuencias_en_1(vector);

	Contextos* ctx_cero = new Contextos();
	ctx_cero->agregar_modelo("0",mod_cero);
	mapa_ordenes.insert(pair<int,Contextos*> (0,ctx_cero));

	// ORDEN 1 a "orden"
	for (unsigned short i = 1; i < orden+1; i++)
	{
		Contextos* contexto_vacio = new Contextos();
		mapa_ordenes.insert(pair<int,Contextos*> (i,contexto_vacio));
	}

}


PPMC::~PPMC()
{
	delete comp_aritmetico;
}


void PPMC::_crear_modelo_vacio (int orden, string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	Contextos* contexto = mapa_ordenes[orden];
	contexto->agregar_modelo(nombre_modelo,modelo);
	mapa_ordenes[orden] = contexto;
}

void PPMC::_guardar_bits(char* bufferComprimido,
		Uint & indiceBufferComprimido,
		BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
		vector<bool> bits_a_emitir)
{

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

//	string contexto_inicial=contexto;
//
//	int res = comprimir(simbolo, orden, contexto, bits_a_emitir);
//
//	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);
//	_actualizar_contexto(orden, simbolo, contexto);bits_a_emitir.clear();
//
//	while (res == RES_ESCAPE)
//	{
//		orden --;
//		if (orden == -1)
//		{
//			contexto = "-1";
//		}
//		else
//			if (orden == 0)
//				contexto = "0";
//			else{
//			contexto.erase(0,1);
//			}
//
////		cout << "INTENTO COMPRIMIR " << (char)simbolo << " EN " << orden << endl;
//		res = comprimir(simbolo,orden,contexto,bits_a_emitir);
//		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);
//
//	}

	vector<bool> bits_a_emitir = comp_aritmetico->comprimir_ultimo_paso();
	_guardar_bits(bufferComprimido,indiceBufferComprimido,buffer_bits,bits_a_emitir);

}

void PPMC::_imprimir_todos_ordenes()
{
	for (int orden = -1; orden < orden_maximo + 1; orden++)
	{
		cout << "---------------- ORDEN " << orden << " ----------------" << endl;
		map<string, ModeloProbabilistico*> mapa_modelos = mapa_ordenes[orden]->get_mapa_modelos();
		map<string, ModeloProbabilistico*>::iterator iterador = mapa_modelos.begin();
		while (iterador != mapa_modelos.end())
		{
			cout << "~~~~~~ CTX " << (*iterador).first << " ~~~~~~" <<endl;
			ModeloProbabilistico* unModelo = (*iterador).second;
			unModelo->imprimir();
			iterador ++;
		}
	}

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

int PPMC::comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;
	ModeloProbabilistico* modelo_actual = NULL;

	mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo_actual);

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;

	if (modelo_actual->get_frecuencia(simbolo) == 0) {
		resultado = RES_ESCAPE;

		cout << "--- EMITO ESCAPE EN ORDEN " << orden << " CON PROBA = " << modelo_actual->get_probabilidad(VALOR_DEL_ESCAPE) << endl;

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

		cout << "--- EMITO " << (char)simbolo << " EN ORDEN " << orden << " CON PROBA = " << modelo_actual->get_probabilidad(simbolo) << endl;

		a_emitir = comp_aritmetico->comprimir(simbolo,cOverflow,cUnderflow);
		if (orden == -1)
			comp_aritmetico->set_modelo(modelo_antes_de_comprimir);
	}

	return resultado;
}

void PPMC::_comprimir_un_caracter(int& orden,Uint simbolo, string& contexto, BufferBits<TAMANIO_BUFFER_BITS_DEFAULT>& buffer_bits,
		vector<bool>& bits_a_emitir,char* bufferComprimido,Uint& indiceBufferComprimido)
{
	if (orden == 0 || orden == -1)
		contexto = utilitarios::int_a_string(orden);

	string maximo_contexto_actual = contexto;

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

}

int PPMC::comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* bufferComprimido)
{
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> buffer_bits;
	string contexto = "";
	int orden = 0;
	Uint indiceBufferComprimido = 0;
	vector<bool> bits_a_emitir;

	for (Uint i = 0; i < tamanioBuffer - 1; i++)
	{
		Uint simbolo = buffer[i];
		cout << "SIMBOLO A COMPRIMIR = " << (char)simbolo << endl;

		_comprimir_un_caracter(orden,simbolo,contexto, buffer_bits,
				bits_a_emitir,bufferComprimido,indiceBufferComprimido);

		orden += 2 + i;
		if (orden > orden_maximo)
			orden = orden_maximo;
	}

	_comprimir_ultimo_paso(buffer[tamanioBuffer-1], orden, contexto, bufferComprimido, indiceBufferComprimido, buffer_bits);

	return indiceBufferComprimido;
}
