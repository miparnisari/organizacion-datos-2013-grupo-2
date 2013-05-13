/*
 * NodoSecuencial.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "NodoSecuencial.h"

NodoSecuencial::NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados)
	:NodoArbol(TIPO_HOJA)
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
	for (unsigned int i = 0; i < vectorRegistros.size(); i ++)
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

	for (unsigned int i = regsQueQuedan; i < vectorRegistros.size() - 1; i ++ )
	{
		regsOverflow.push_back(vectorRegistros.at(i));
	}
	vectorRegistros.erase(vectorRegistros.begin()+regsQueQuedan,vectorRegistros.end());
}

int NodoSecuencial::insertar(RegistroClave & registro, std::vector<RegistroClave> & regsOverflow)
{
	RegistroClave* regAbuscar = new RegistroClave();
	ClaveX claveAbuscar;
	registro.get_clave(claveAbuscar);
	regAbuscar->set_clave(claveAbuscar);
	int resBuscar = buscar(regAbuscar);
	if (resBuscar == RES_ERROR)
	{
		delete (regAbuscar);
		return RES_ERROR;
	}
	else if (resBuscar == RES_OK)
	{
		delete (regAbuscar);
		return RES_RECORD_EXISTS;
	}
	else // (NO ESTA)
	{
		vectorRegistros.push_back(registro);
		bytesOcupados += registro.get_tamanio_empaquetado();
		std::sort(vectorRegistros.begin(), vectorRegistros.end());

		delete (regAbuscar);
		if (bytesOcupados > maxCantidadBytesOcupados)
		{
			__resolver_overflow(regsOverflow);
			return RES_OVERFLOW;
		}
		return RES_OK;
	}
}

int NodoSecuencial::eliminar(const ClaveX clave, std::vector<RegistroClave>& regsUnderflow)
{
	RegistroClave* registro = new RegistroClave();
	registro->set_clave(clave);
	int posicionEliminar = buscar(registro);
	if (posicionEliminar >= 0)
	{
		bytesOcupados -= vectorRegistros.at(posicionEliminar).get_tamanio_empaquetado();
		vectorRegistros.erase(vectorRegistros.begin()+posicionEliminar);
		delete(registro);
		if (bytesOcupados < minCantidadBytesOcupados)
		{
			__resolver_underflow(regsUnderflow);
			return RES_UNDERFLOW;
		}
		return RES_OK;

	}
	else // Se quiso eliminar algo que no estaba
	{
		delete(registro);
		return RES_RECORD_DOESNT_EXIST;
	}

}

int NodoSecuencial::buscar(RegistroClave* regDevuelto)
{
	if (regDevuelto == NULL)
		return RES_ERROR;
	ClaveX clave;
	regDevuelto->get_clave(clave);

	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		ClaveX claveAlmacenada;
		vectorRegistros.at(i).get_clave(claveAlmacenada);
		if (clave == claveAlmacenada)
		{
			for (unsigned int j = 1; j < vectorRegistros.at(i).get_cantidad_campos(); j++)
			{
//				std::cout << "tamanio campo = " << registroAlmacenado.get_tamanio_campo(j) << std::endl;
				char* buffer = new char[vectorRegistros.at(i).get_tamanio_campo(j)+1]();
				buffer[vectorRegistros.at(i).get_tamanio_campo(j)] = '\0';
				vectorRegistros.at(i).recuperar_campo(buffer, j);
//				std:: cout << "buffer" << buffer << std::endl;
//				std:: cout << "strlen" << strlen(buffer) << std::endl;
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

	// Empaqueto los registros del nodo
	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		bloque->agregar_registro(&(vectorRegistros.at(i)));
	}
	RegistroClave regConPuntero;
	regConPuntero.agregar_campo((char*)&proximoNodo, sizeof(proximoNodo));
	bloque->agregar_registro(&regConPuntero);

	return RES_OK;

}

int NodoSecuencial::desempaquetar(const Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;
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
