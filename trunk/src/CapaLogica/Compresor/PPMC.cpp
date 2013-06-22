#include "PPMC.h"


PPMC::PPMC(unsigned short orden) : Compresor()
{
	orden_maximo = orden;
	comp_aritmetico = new Aritmetico();
	contextos = new Contextos();

	// ORDEN -1
	ModeloProbabilistico* mod_menos_uno = new ModeloProbabilistico(TAMANIO_ALFABETO-1);
	mod_menos_uno->inicializar_frecuencias_en_1();

	contextos->agregar_modelo("-1",mod_menos_uno);

	// ORDEN 0
	_crear_modelo_vacio("0");
}


PPMC::~PPMC()
{
	delete contextos;
	delete comp_aritmetico;
}

void PPMC::_crear_modelo_vacio (string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	contextos->agregar_modelo(nombre_modelo,modelo);
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

std::vector<bool> PPMC::_comprimir_ultimo_paso(string contexto)
{
	ModeloProbabilistico* modelo_actual = NULL;
	int res = contextos->devolver_modelo(contexto,&modelo_actual);
	if (res == RES_ERROR)
	{
		_crear_modelo_vacio(contexto);
		contextos->devolver_modelo(contexto,&modelo_actual);
	}

	comp_aritmetico->set_modelo (modelo_actual);

	vector<bool> bits_a_emitir = comp_aritmetico->comprimir_ultimo_paso();

	return bits_a_emitir;
}

void PPMC::_actualizar_contexto(int num_contexto_actual, Uint simbolo, string contexto_del_simbolo)
{

	// Caso en el que se emitio ESCAPE y era el unico simbolo posible
	// Entonces aumentamos la frecuencia del caracter que acabamos de leer
	cout << "INCREMENTAR FRECUENCIA DE " << (char)simbolo << " EN " << contexto_del_simbolo << endl;
	contextos->incrementar_frecuencia(simbolo,contexto_del_simbolo);

	if (num_contexto_actual != orden_maximo)
	{
		// En el contexto superior, creamos el contexto "contexto + simbolo"
		// solo con el ESCAPE
		ModeloProbabilistico* modelo_contexto_superior;

		string ctx = "";
		if (num_contexto_actual != -1)
			ctx += contexto_del_simbolo;
		ctx += simbolo;

		cout << "CREAR CONTEXTO " << ctx << " SOLO CON ESCAPE = 1" << endl;
		int res = contextos->devolver_modelo(ctx,&modelo_contexto_superior);
		if (res == RES_ERROR)
		{
			_crear_modelo_vacio(ctx);
		}
	}

}

int PPMC::comprimir (const Uint simbolo, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;
	ModeloProbabilistico* modelo_actual = NULL;
	int res = contextos->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	if (res == RES_ERROR)
	{
		cout << "CREAR CONTEXTO " << contexto_del_simbolo << endl;
		_crear_modelo_vacio(contexto_del_simbolo);
		contextos->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	}

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;
	if (modelo_actual->get_frecuencia(simbolo) == 0) {
		resultado = RES_ESCAPE;


		if (modelo_actual->get_frecuencia(VALOR_DEL_ESCAPE) == 1  && modelo_actual->calcular_total_frecuencias() == 1)
		{
			// Si el escape es el unico simbolo no se va a emitir nada asi que no hay que llamar a comprimir
			int num_contexto_actual = utilitarios::string_a_int(contexto_del_simbolo);
			_actualizar_contexto(num_contexto_actual,simbolo, contexto_del_simbolo);
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
	string contexto = "0"; // "ABC"
	int num_contexto = 0;
	Uint indiceBufferComprimido = 0;
	vector<bool> bits_a_emitir;

	for (Uint i = 0; i < tamanioBuffer; i++)
	{
		Uint simbolo = buffer[i];
		cout << "ORDEN =" << num_contexto << endl;
		cout << "CONTEXTO = " << contexto << endl;
		cout << "SIMBOLO A COMPRIMIR =" << (char)simbolo << endl;
		int res = comprimir(simbolo,contexto, bits_a_emitir);

		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

		num_contexto --;

		while (res == RES_ESCAPE)
		{
			cout << "EMITI ESCAPE" << endl;
			char primer_char_ctx = contexto[0];
			contexto.erase(0,1); // "BC"
			contexto = utilitarios::int_a_string(num_contexto);
			cout << "INTENTO COMPRIMIR " << simbolo << " EN " << contexto << endl;
			res = comprimir(simbolo,contexto,bits_a_emitir);
			_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

			if (num_contexto == -1) {
				contexto = primer_char_ctx;
				contexto += simbolo;
				num_contexto += 2 + i;
				if (num_contexto > orden_maximo)
					num_contexto = orden_maximo;

				cout << "NUEVO CONTEXTO = " << contexto << endl;
				cout << "NUEVO ORDEN = " << num_contexto << endl;
				break;
			}
			else
			{
				num_contexto --;
				contexto += simbolo;
			}
		}


	}

	bits_a_emitir = _comprimir_ultimo_paso(contexto);
	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

	return indiceBufferComprimido;
}
