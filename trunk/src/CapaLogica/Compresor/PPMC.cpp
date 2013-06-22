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
}

void PPMC::_crear_modelo_vacio (string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	contextos->agregar_modelo(nombre_modelo,modelo);
}

/*
 * El resultado puede ser Escape!
 */
Uint PPMC::comprimir (const double proba, std::string contexto_del_simbolo)
{
	ModeloProbabilistico* modelo_actual = NULL;
	int res = contextos->devolver_modelo(contexto_del_simbolo,&modelo_actual);
	if (res == RES_ERROR)
	{
		_crear_modelo_vacio(contexto_del_simbolo);
	}
	comp_aritmetico->set_modelo (modelo_actual);

	Uint resultado = modelo_actual->obtener_simbolo(proba);
	return resultado;
}

int PPMC::comprimir_todo(const char* buffer,const unsigned int tamanioBuffer,char* resultado)
{
	string contexto = "";

	for (Uint i = 0; i < tamanioBuffer; i++)
	{
		Uint simbolo = buffer[i];
//		if (modelo_actual->es_frecuencia_cero (simbolo))
		{
			// hay q emitir esc
			// y disminuir el orden
		}

	}
}
