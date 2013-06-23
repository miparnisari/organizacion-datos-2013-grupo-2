#include "PPMC.h"


PPMC::PPMC(unsigned short orden) : Compresor()
{
	orden_maximo = orden;
	comp_aritmetico = new Aritmetico();
//	contextos = new Contextos();

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
	for (unsigned short i = 1; i < orden; i++)
	{
		Contextos* contexto_vacio = new Contextos();
		contexto_vacio->agregar_modelo("0",NULL);

		mapa_ordenes.insert(pair<int,Contextos*> (i,contexto_vacio));
	}

}


PPMC::~PPMC()
{
//	delete contextos;
	delete comp_aritmetico;
}

void PPMC::_crear_modelo_vacio (int orden, string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	Contextos* contexto = new Contextos();
	contexto->agregar_modelo(nombre_modelo,modelo);
	mapa_ordenes.insert(pair<int,Contextos*> (orden,contexto));
}

void PPMC::_guardar_bits(char* bufferComprimido,
		Uint & indiceBufferComprimido,
		BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> & buffer_bits,
		vector<bool> bits_a_emitir)
{

	const Uint CANTIDAD_BITS_EMITIR= bits_a_emitir.size();
	if(CANTIDAD_BITS_EMITIR== 0)
		return;


	for(Uint i=0;i<CANTIDAD_BITS_EMITIR;i++){
		bool bit= bits_a_emitir.at(i);
		buffer_bits.agregar_bit(bit);
	}

	if(!buffer_bits.completa_octeto())
		return;

	char* puntero= bufferComprimido+indiceBufferComprimido;
	Uint const CANTIDAD_BYTES_ESCRIBIR= buffer_bits.get_cantidad_bytes();
	buffer_bits.dump( puntero );
	indiceBufferComprimido+= CANTIDAD_BYTES_ESCRIBIR;


//	Uint CANTIDAD_BYTES_ESCRIBIR= buffer_bits.get_cantidad_bytes();
//	for(Uint i=0;i<CANTIDAD_BYTES_ESCRIBIR;i++){
//		Byte byte;
//		buffer_bits.pop_byte(byte);
//		bufferComprimido[indiceBufferComprimido]= (char)byte;
//		indiceBufferComprimido++;
//	}



}

std::vector<bool> PPMC::_comprimir_ultimo_paso(int orden, string contexto)
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

	return bits_a_emitir;
}

void PPMC::_actualizar_contexto(int orden, Uint simbolo, string contexto_del_simbolo)
{

	// Caso en el que se emitio ESCAPE y era el unico simbolo posible
	// Entonces aumentamos la frecuencia del caracter que acabamos de leer
	cout << "INCREMENTAR FRECUENCIA DE " << (char)simbolo << " EN " << contexto_del_simbolo << endl;
	mapa_ordenes[orden]->incrementar_frecuencia(simbolo,contexto_del_simbolo);

	if (orden != orden_maximo)
	{
		// En el contexto superior, creamos el contexto "contexto + simbolo"
		// solo con el ESCAPE
		ModeloProbabilistico* modelo_contexto_superior;

		string ctx = "";
		if ((orden != -1) && (orden != 0))
			ctx += contexto_del_simbolo;
		ctx += simbolo;

		cout << "CREAR CONTEXTO " << ctx << " EN ORDEN " << orden +1 << endl;


		int res = mapa_ordenes[orden+1]->devolver_modelo(ctx,&modelo_contexto_superior);
		if (res == RES_ERROR)
		{
			_crear_modelo_vacio(orden+1,ctx);
		}
	}

}

int PPMC::comprimir (const Uint simbolo, int orden, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;
	ModeloProbabilistico* modelo_actual = NULL;
	int res = mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	if (res == RES_ERROR)
	{
		if (orden == 0 || orden == -1)
			contexto_del_simbolo = utilitarios::int_a_string(orden);
//		cout << "CREAR CONTEXTO " << contexto_del_simbolo << " EN ORDEN " << orden << endl;
		_crear_modelo_vacio(orden,contexto_del_simbolo);
		mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	}

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;
	if (modelo_actual->get_frecuencia(simbolo) == 0) {
		resultado = RES_ESCAPE;


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
		a_emitir = comp_aritmetico->comprimir(simbolo,cOverflow,cUnderflow);
	}

	return resultado;
}

int PPMC::comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* bufferComprimido)
{
	BufferBits<TAMANIO_BUFFER_BITS_DEFAULT> buffer_bits;
	string contexto = ""; // "ABC"
	int orden = 0;
	Uint indiceBufferComprimido = 0;
	vector<bool> bits_a_emitir;

	for (Uint i = 0; i < tamanioBuffer; i++)
	{
		if (orden == 0 || orden == -1)
			contexto = utilitarios::int_a_string(orden);
		Uint simbolo = buffer[i];
		cout << "ORDEN =" << orden << endl;
		cout << "CONTEXTO = " << contexto << endl;
		cout << "SIMBOLO A COMPRIMIR =" << (char)simbolo << endl;
		int res = comprimir(simbolo, orden, contexto, bits_a_emitir);

		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

		orden --;

		while (res == RES_ESCAPE)
		{
			_actualizar_contexto(orden+1,simbolo, contexto);

			cout << "EMITI ESCAPE EN ORDEN " << orden+1 << endl;
			string copia_contexto = contexto;
			if (orden == -1)
				contexto.erase(0,2);
			if (orden == 0)
				contexto.erase(0,1);

			cout << "INTENTO COMPRIMIR " << (char)simbolo << " EN " << orden << endl;
			res = comprimir(simbolo,orden,contexto,bits_a_emitir);
			_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

			if (orden == -1) {
				contexto += simbolo;
				orden += 2 + i;
				if (orden > orden_maximo)
					orden = orden_maximo;

				cout << "NUEVO CONTEXTO = " << contexto << endl;
				cout << "NUEVO ORDEN = " << orden << endl;
				break;
			}
			else
			{
				orden --;
				contexto += simbolo;
			}
		}


	}

	bits_a_emitir = _comprimir_ultimo_paso(orden,contexto);
	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

	return indiceBufferComprimido;
}
