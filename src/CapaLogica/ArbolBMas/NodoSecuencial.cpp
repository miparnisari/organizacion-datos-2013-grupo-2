/*
 * NodoSecuencial.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "NodoSecuencial.h"
#include "../../CapaFisica/RegistroVariable.h"

NodoSecuencial::NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados)
	:Nodo('S')
{
	minCantidadBytesOcupados = minBytesOcupados;
	maxCantidadBytesOcupados = maxBytesOcupados;
	bytesOcupados = 0;
	proximoNodo = -1;
}

NodoSecuencial::~NodoSecuencial() {
	// TODO Auto-generated destructor stub
}

int NodoSecuencial::empaquetar(Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;

	int tam_puntero = sizeof(this->proximoNodo);
	int tam_registros = 0;
	int i;
	for (i = 0 ; i < vectorRegistros.size() ; i++)
		tam_registros += vectorRegistros.at(i).get_tamanio_empaquetado();
	char* buffer = new char[tam_registros + tam_puntero]();

	// Empaqueto los registros del nodo
	for (i = 0; i < vectorRegistros.size(); i++)
	{
		RegistroVariable reg = vectorRegistros.at(i);
		char* bufferRegistro = new char[reg.get_tamanio_empaquetado()]();
		reg.empaquetar(bufferRegistro);
		strcat(buffer,bufferRegistro);
		delete[] bufferRegistro;
	}

	strcat(buffer,(char*)&proximoNodo);
	bloque->empaquetar(buffer);
	delete[] buffer;

	return RES_OK;

}
	int desempaquetar(const Bloque*);

