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
	maxCantidadBytesOcupados = maxBytesOcupados + sizeof(TIPO_HOJA) + sizeof(proximoNodo);
	bytesOcupados = sizeof(TIPO_HOJA) + sizeof(proximoNodo);
	proximoNodo = -1;
}

NodoSecuencial::NodoSecuencial(const NodoSecuencial& otro)
	:NodoArbol(TIPO_HOJA)
{
	maxCantidadBytesOcupados = otro.get_max_bytes();
	minCantidadBytesOcupados = otro.get_min_bytes();
	bytesOcupados = otro.get_bytes_ocupados();
	proximoNodo = otro.get_proximo_nodo();
	vectorRegistros = otro.get_registros();
}

NodoSecuencial& NodoSecuencial::operator=(const NodoSecuencial& otro)
{
	if (this != &otro)
	{
		maxCantidadBytesOcupados = otro.get_max_bytes();
		minCantidadBytesOcupados = otro.get_min_bytes();
		bytesOcupados = otro.get_bytes_ocupados();
		proximoNodo = otro.get_proximo_nodo();
		vectorRegistros.clear();
		vectorRegistros = otro.get_registros();
	}

	return *this;
}

std::vector<RegistroClave> NodoSecuencial::get_registros() const
{
	return this->vectorRegistros;
}



bool NodoSecuencial::tiene_carga_minima ()
{
	if (vectorRegistros.size() == 1)
		return true;

	return false;
}/* FIXME refactorizar y tener en cuenta que la carga minima puede ser distinta de 0. */

NodoSecuencial::~NodoSecuencial()
{
	vectorRegistros.clear();
}

void NodoSecuencial::__limpiar()
{
	bytesOcupados = sizeof(TIPO_HOJA) + sizeof(proximoNodo);
	proximoNodo = -1;
	vectorRegistros.clear();
}

unsigned int NodoSecuencial::get_max_bytes()const
{
	return this->maxCantidadBytesOcupados;
}
unsigned int NodoSecuencial::get_min_bytes()const
{
	return this->minCantidadBytesOcupados;
}

unsigned int NodoSecuencial::get_cantidad_registros()const
{
	return vectorRegistros.size();
}

bool NodoSecuencial::esta_vacio()
{
	return (vectorRegistros.size() == 0);
}

unsigned int NodoSecuencial::get_bytes_ocupados()const
{
	return bytesOcupados;
}

TipoPuntero NodoSecuencial::get_proximo_nodo()const
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

void NodoSecuencial::imprimir()
{
	std::cout << "SECUENCIAL------";
	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		vectorRegistros.at(i).get_clave().imprimir_dato();
	}
	std::cout << "-----";
	std::cout << "PROX=" << proximoNodo << std::endl;
}

void NodoSecuencial::__resolver_overflow(std::vector<RegistroClave> & regsOverflow)
{
	regsOverflow.clear();

	unsigned int mitad = (bytesOcupados-sizeof(tipoNodo)-sizeof(proximoNodo)) / 2.0;

	unsigned int regsQueQuedan = 0;
	unsigned int cantidadBytesRegsQueQuedan = 0;
	unsigned int i = 0;
	while (cantidadBytesRegsQueQuedan < mitad)
	{
		cantidadBytesRegsQueQuedan += vectorRegistros.at(i).get_tamanio_empaquetado();
		i++;
		regsQueQuedan ++;
	}

	for (unsigned int i = regsQueQuedan; i < vectorRegistros.size(); i ++ )
	{
		regsOverflow.push_back(vectorRegistros.at(i));
	}
	vectorRegistros.erase(vectorRegistros.begin()+regsQueQuedan,vectorRegistros.end());
}

int NodoSecuencial::insertar(const RegistroClave & registro, std::vector<RegistroClave> & regsOverflow)
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
			std::cout<<"resuelve overfloW!\n";
			__resolver_overflow(regsOverflow);
			return RES_OVERFLOW;
		}
		return RES_OK;
	}
}

int NodoSecuencial::eliminar(const ClaveX & clave, std::vector<RegistroClave>& regsUnderflow)
{
	int posicionEliminar = buscar(clave,NULL);
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

int NodoSecuencial::buscar(const ClaveX & claveBuscada, RegistroClave** regDevuelto)
{
	for (unsigned int i = 0; i < vectorRegistros.size(); i++)
	{
		ClaveX* claveAlmacenada = new ClaveX(vectorRegistros.at(i).get_clave());

		if (claveBuscada == *claveAlmacenada)
		{
			if (regDevuelto != NULL)
				*regDevuelto = new RegistroClave(vectorRegistros.at(i));

			delete (claveAlmacenada);
			return i;
		}
		delete (claveAlmacenada);
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
	header.recuperar_campo((char*)&tipoNodo,0);
	header.recuperar_campo((char*)&proximoNodo, 1);

	bytesOcupados = sizeof(tipoNodo) + sizeof(proximoNodo);


	// Desempaqueto los registros
	for (int i = 1; i < q; i ++)
	{
		RegistroClave regActual;
		bloque->recuperar_registro(&regActual, i);
		vectorRegistros.push_back(regActual);
		bytesOcupados += regActual.get_tamanio_empaquetado();
	}

	return RES_OK;
}
