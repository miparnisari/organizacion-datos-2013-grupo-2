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


int PPMC::comprimir (const Uint simbolo, std::string contexto_del_simbolo, std::vector<bool>& a_emitir)
{
	int resultado = RES_OK;
	ModeloProbabilistico* modelo_actual = NULL;
	int res = contextos->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	if (res == RES_ERROR)
	{
		_crear_modelo_vacio(contexto_del_simbolo);
		contextos->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	}

	comp_aritmetico->set_modelo (modelo_actual);

	Byte cOverflow, cUnderflow;
	if (modelo_actual->get_frecuencia(simbolo) == 0) {
		resultado = RES_ESCAPE;

		// Si el escape NO es el unico simbolo, hay que comprimirlo
		// Si es el unico simbolo no se va a emitir nada asi que no hay que llamar a comprimir
		if (modelo_actual->get_frecuencia(VALOR_DEL_ESCAPE) == 1  && modelo_actual->calcular_total_frecuencias() == 1)
			a_emitir = comp_aritmetico->comprimir(VALOR_DEL_ESCAPE,cOverflow,cUnderflow);
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

		int res = comprimir(simbolo,contexto, bits_a_emitir);

		_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

		while (res == RES_ESCAPE)
		{
			contexto.erase(0,1); // "BC"
			if (contexto == "0")
			{
				num_contexto = -1;
			}
			contexto = utilitarios::int_a_string(num_contexto);

			res = comprimir(simbolo,contexto,bits_a_emitir);
			_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

			if (num_contexto == -1)
				num_contexto = orden_maximo;
			else
				num_contexto --;

		}
	}

	bits_a_emitir = _comprimir_ultimo_paso(contexto);
	_guardar_bits(bufferComprimido, indiceBufferComprimido, buffer_bits, bits_a_emitir);

	return RES_OK;
}
