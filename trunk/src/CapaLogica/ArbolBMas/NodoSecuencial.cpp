/*
 * NodoSecuencial.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "NodoSecuencial.h"

NodoSecuencial::NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados)
	:Nodo('S')
{
	minCantidadBytesOcupados = minBytesOcupados;
	maxCantidadBytesOcupados = maxBytesOcupados;
	bytesOcupados = 0;
	proximoNodo = -1;
}

NodoSecuencial::~NodoSecuencial()
{

}

unsigned int NodoSecuencial::get_cantidad_registros()
{
	return vectorRegistros.size();
}

bool NodoSecuencial::esta_vacio()
{
	return (vectorRegistros.size() == 0);
}

TipoPuntero NodoSecuencial::get_proximo_nodo()
{
	return proximoNodo;
}

void NodoSecuencial::set_proximo_nodo(TipoPuntero p_prox)
{
	proximoNodo = p_prox;
}

void NodoSecuencial::__resolver_underflow(std::vector<RegistroClave> & regsUnderflow)
{
	bytesOcupados = 0;
	for (int i = 0; i < vectorRegistros.size(); i ++)
	{
		regsUnderflow.push_back(vectorRegistros.at(i));
	}

	vectorRegistros.erase(vectorRegistros.begin(), vectorRegistros.end());
}

void NodoSecuencial::__resolver_overflow(std::vector<RegistroClave> & regsOverflow)
{
	unsigned int mitad = ceil(bytesOcupados / 2.0);

	unsigned int regsQueQuedan = 0;
	unsigned int cantidadBytesRegsQueQuedan = 0;
	unsigned int i = 0;
	while (cantidadBytesRegsQueQuedan < mitad)
	{
		cantidadBytesRegsQueQuedan += vectorRegistros.at(i).get_tamanio_empaquetado();
		i++;
		regsQueQuedan ++;
	}

	for (int i = regsQueQuedan; i < vectorRegistros.size() - 1; i ++ )
	{
		regsOverflow.push_back(vectorRegistros.at(i));
	}
	vectorRegistros.erase(vectorRegistros.begin()+regsQueQuedan,vectorRegistros.end());
}/* Devuelve en un vector los registros que se sacaron del nodo. */

int NodoSecuencial::insertar(RegistroClave* registro, std::vector<RegistroClave> & regsOverflow)
{
	if (registro == NULL)
		return RES_ERROR;

	int resBuscar = buscar(registro);
	if (resBuscar == RES_ERROR)
	{
		return RES_ERROR;
	}
	else if (resBuscar == RES_OK)
	{
		return RES_RECORD_EXISTS;
	}
	else // (NO ESTA)
	{
		vectorRegistros.push_back(*registro);
		bytesOcupados += registro->get_tamanio_empaquetado();
		std::sort(vectorRegistros.begin(), vectorRegistros.end());

		if (bytesOcupados > maxCantidadBytesOcupados)
		{
			__resolver_overflow(regsOverflow);
			return RES_OVERFLOW;
		}
		return RES_OK;
	}
} /* Inserta un registro en el nodo. Si ya estaba, devuelve RES_RECORD_EXISTS.
Si no estaba, lo inserta. Si al insertar hubo overflow, devuelve RES_OVERFLOW.
Si no, devuelve RES_OK.
Si hubo error, devuelve RES_ERROR. */

int NodoSecuencial::eliminar(ClaveX* clave, std::vector<RegistroClave>& regsUnderflow)
{
	if (clave == NULL)
		return RES_ERROR;

	RegistroClave registro;
	registro.set_clave(clave);
	int posicionEliminar = buscar(&registro);
	if (posicionEliminar >= 0)
	{
		bytesOcupados -= vectorRegistros.at(posicionEliminar).get_tamanio_empaquetado();
		vectorRegistros.erase(vectorRegistros.begin()+posicionEliminar);

		if (bytesOcupados < minCantidadBytesOcupados)
		{
			__resolver_underflow(regsUnderflow);
			return RES_UNDERFLOW;
		}
		return RES_OK;

	}
	else // Se quiso eliminar algo que no estaba
	{
		return RES_RECORD_DOESNT_EXIST;
	}

}

int NodoSecuencial::buscar(RegistroClave* regDevuelto)
{
	if (regDevuelto == NULL)
		return RES_ERROR;
	ClaveX clave;
	if (regDevuelto->get_clave(&clave) != RES_OK)
		return RES_ERROR;

	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		RegistroClave registroAlmacenado = vectorRegistros.at(i);
		ClaveX claveAlmacenada;
		registroAlmacenado.get_clave(&claveAlmacenada);
		if (clave == claveAlmacenada)
		{
			for (unsigned int j = 1; j < registroAlmacenado.get_cantidad_campos(); j++)
			{
				char* buffer = new char[registroAlmacenado.get_tamanio_campo(j)]();
				registroAlmacenado.recuperar_campo(buffer, j);
				regDevuelto->agregar_campo(buffer,strlen(buffer));
				delete[] buffer;
			}
			return i;
		}
	}

	return RES_RECORD_DOESNT_EXIST;
}

int NodoSecuencial::empaquetar(Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;

	int tam_registros = 0;
	unsigned int i;
	for (i = 0 ; i < vectorRegistros.size() ; i++)
		tam_registros += vectorRegistros.at(i).get_tamanio_empaquetado();

	// Empaqueto los registros del nodo
	for (i = 0; i < vectorRegistros.size(); i++)
	{
		RegistroClave reg = vectorRegistros.at(i);
		bloque->agregar_registro(&reg);
	}
	RegistroClave regConPuntero;
	regConPuntero.agregar_campo((char*)&proximoNodo, sizeof(proximoNodo));
	bloque->agregar_registro(&regConPuntero);

	return RES_OK;

}

int NodoSecuencial::desempaquetar(const Bloque* bloque)
{
	if (bloque == NULL)
		return NULL;
	if (bloque->esta_vacio())
		return RES_OK;

	unsigned short int q = bloque->get_cantidad_registros_almacenados();

	for (int i = 0 ; i < q-1; i ++)
	{
		RegistroClave regActual;
		bloque->recuperar_registro(&regActual, i);

		vectorRegistros.push_back(regActual);
		bytesOcupados += regActual.get_tamanio_empaquetado();
	}

	RegistroClave regConPuntero;
	bloque->recuperar_registro(&regConPuntero, q-1);
	regConPuntero.empaquetar((char*)&proximoNodo);
	bytesOcupados += sizeof(proximoNodo);

	return RES_OK;
}
