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
	bytesOcupados = sizeof(TIPO_HOJA) + sizeof(proximoNodo);
	proximoNodo = -1;
}

NodoSecuencial::~NodoSecuencial()
{

}

void NodoSecuencial::__limpiar()
{
	bytesOcupados = sizeof(TIPO_HOJA) + sizeof(proximoNodo);
	proximoNodo = -1;
	vectorRegistros.clear();
}

unsigned int NodoSecuencial::get_cantidad_registros()
{
	return vectorRegistros.size();
}

bool NodoSecuencial::esta_vacio()
{
	return (vectorRegistros.size() == 0);
}

unsigned int NodoSecuencial::get_bytes_ocupados()
{
	return bytesOcupados;
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
	int resBuscar = buscar(registro.get_clave(),NULL);
	if (resBuscar == RES_ERROR)
		return RES_ERROR;

	else if (resBuscar == RES_OK)
		return RES_RECORD_EXISTS;


	else // (LA CLAVE NO ESTA EN EL NODO)
	{
		vectorRegistros.push_back(registro);
		bytesOcupados += registro.get_tamanio_empaquetado();
		std::sort(vectorRegistros.begin(), vectorRegistros.end());

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
	RegistroClave* registro = NULL;
	int posicionEliminar = buscar(clave,&registro);
	if (posicionEliminar >= 0 && registro != NULL)
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

int NodoSecuencial::buscar(ClaveX claveBuscada, RegistroClave** regDevuelto)
{
	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		ClaveX claveAlmacenada (vectorRegistros.at(i).get_clave());

		if (claveBuscada == claveAlmacenada)
		{
			if (regDevuelto != NULL)
				*regDevuelto = new RegistroClave(vectorRegistros.at(i));

//			for (unsigned int j = 1; j < vectorRegistros.at(i).get_cantidad_campos(); j++)
//			{

//				//				std::cout << "tamanio campo = " << registroAlmacenado.get_tamanio_campo(j) << std::endl;
//				char* buffer = new char[vectorRegistros.at(i).get_tamanio_campo(j)+1]();
//				buffer[vectorRegistros.at(i).get_tamanio_campo(j)] = '\0';
//				vectorRegistros.at(i).recuperar_campo(buffer, j);
////				std:: cout << "buffer" << buffer << std::endl;
////				std:: cout << "strlen" << strlen(buffer) << std::endl;
//				regDevuelto->agregar_campo(buffer,strlen(buffer));
//				delete[] buffer;
//			}
			return i;
		}
	}

	return RES_RECORD_DOESNT_EXIST;
}/* POSTCONDICION: liberar la memoria de regDevuelto. */

int NodoSecuencial::empaquetar(Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;

	//Empaqueto el tipo de nodo y el puntero al proximo nodo
	RegistroVariable header;
	header.agregar_campo((char*)&tipoNodo, sizeof(tipoNodo));
	header.agregar_campo((char*)&proximoNodo, sizeof(proximoNodo));
	bloque->agregar_registro(&header);

	// Empaqueto los registros del nodo
	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		bloque->agregar_registro(&(vectorRegistros.at(i)));
	}

	return RES_OK;

}/* POSTCONDICION: empaquetar el bloque. */

int NodoSecuencial::desempaquetar(const Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;
	if (bloque->esta_vacio())
		return RES_OK;

	__limpiar();
	unsigned short int q = bloque->get_cantidad_registros_almacenados();

	// Desempaqueto el header (tipo nodo + puntero proximo nodo)
	RegistroVariable header;
	bloque->recuperar_registro(&header, 0);
	header.recuperar_campo((char*)&tipoNodo,sizeof(tipoNodo));
	header.recuperar_campo((char*)&proximoNodo, sizeof(proximoNodo));

	bytesOcupados = sizeof(tipoNodo) + sizeof(proximoNodo);


	// Desempaqueto los registros
	for (int i = 0 ; i < q-1; i ++)
	{
		RegistroClave regActual;
		bloque->recuperar_registro(&regActual, i);
		vectorRegistros.push_back(regActual);
		bytesOcupados += regActual.get_tamanio_empaquetado();
	}

	return RES_OK;
}
