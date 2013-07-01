/*
 * ListaPosicionesTerminosEnDocumentos.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#include "ListaPosicionesTerminosEnDocumentos.h"

ListaPosicionesTerminosEnDocumentos::ListaPosicionesTerminosEnDocumentos() {

}

ListaPosicionesTerminosEnDocumentos::~ListaPosicionesTerminosEnDocumentos() {
}

int ListaPosicionesTerminosEnDocumentos::crear(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.crear_archivo(nombre_archivo);
	return res;
}

int ListaPosicionesTerminosEnDocumentos::eliminar()
{
	int res = archivo.eliminar_archivo(nombre_archivo);
	return res;
}

int ListaPosicionesTerminosEnDocumentos::abrir(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.abrir_archivo(nombre_archivo);
	return res;
}

int ListaPosicionesTerminosEnDocumentos::cerrar()
{
	return RES_OK;
}

int ListaPosicionesTerminosEnDocumentos::ordenar_por_ID_termino()
{
	OrdenamientoExterno ordenador (nombre_archivo);

	// Ordena por ID_termino y por ID_doc
	ordenador.ordenar_archivo();

	return RES_OK;
}

int ListaPosicionesTerminosEnDocumentos::agregar
	(const ListaPosicionesTerminosEnDocumentos_t & lista)
{

	RegistroVariable registro;

	registro.agregar_campo((char*)&lista.id_termino, sizeof(lista.id_termino));
	registro.agregar_campo((char*)&lista.id_doc,sizeof(lista.id_doc));
	registro.agregar_campo((char*)&lista.pos_termino,sizeof(lista.pos_termino));

	return archivo.agregar_registro(&registro);

}

int ListaPosicionesTerminosEnDocumentos::devolver
	(int posicion, ListaPosicionesTerminosEnDocumentos_t & lista )
{

	lista.id_termino = -1;
	lista.id_doc = -1;
	lista.pos_termino = -1;

	RegistroVariable registro;

	int res = archivo.get_registro(&registro,posicion);

	if (res != RES_ERROR)
	{
		registro.recuperar_campo((char*)&lista.id_termino,0);
		registro.recuperar_campo((char*)&lista.id_doc,1);
		registro.recuperar_campo((char*)&lista.pos_termino,2);

		return RES_OK;
	}
	else
	{
		std::string mensaje;
		mensaje = "Error al recuperar posicion = ";
		mensaje	+= utilitarios::int_a_string(posicion);
		Logger::error("IndiceInvertido",mensaje);
	}

	return RES_ERROR;
}
