/*
 * ListaInvertida.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#include "ListaInvertida.h"

ListaInvertida::ListaInvertida() {

}

ListaInvertida::~ListaInvertida() {
}

int ListaInvertida::crear(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.crear_archivo(nombre_archivo);
	return res;
}

int ListaInvertida::eliminar()
{
	int res = archivo.eliminar_archivo(nombre_archivo);
	return res;
}

int ListaInvertida::abrir(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.abrir_archivo(nombre_archivo);
	return res;
}

int ListaInvertida::cerrar()
{
	return RES_OK;
}

ReferenciaListaInvertida_t ListaInvertida::agregar (const std::vector<ListaInvertida_t> & listas)
{
	// Primero se guarda en cuantos documentos aparece el termino
	const unsigned int CANTIDAD = listas.size();
	RegistroVariable registro_cantidad;
	registro_cantidad.agregar_campo((char*)&CANTIDAD,sizeof(CANTIDAD));

	ReferenciaListaInvertida_t offset_inicio = archivo.get_cantidad_registros_ocupados();

	archivo.agregar_registro(&registro_cantidad);


	// Luego se guarda IDdocumento_t, CantidadPosiciones_t,ReferenciaListaPos_t
	for (unsigned int i = 0; i < CANTIDAD; i++)
	{
		RegistroVariable registro;
		IDdocumento_t id_doc = (listas[i].id_doc);
		CantidadPosiciones_t largo_lista = (listas[i].cantidad_pos);
		ReferenciaListaPos_t ref_lista_pos = (listas[i].ref_lista_pos);

		registro.agregar_campo((char*)&id_doc,sizeof(id_doc));
		registro.agregar_campo((char*)&largo_lista,sizeof(largo_lista));
		registro.agregar_campo((char*)&ref_lista_pos,sizeof(ref_lista_pos));

		archivo.agregar_registro(&registro);
	}

	return offset_inicio;
}

std::vector<ListaInvertida_t> ListaInvertida::devolver (const ReferenciaListaInvertida_t ref_lista)
{
	std::vector<ListaInvertida_t> vector_lista_invertida;

	RegistroVariable registro_cantidad;

	int res = archivo.get_registro_ocupado(&registro_cantidad,ref_lista);

	if (res != RES_ERROR)
	{
		int CANTIDAD = -1;
		registro_cantidad.recuperar_campo((char*)&CANTIDAD,0);

		for (int i = 1; i < CANTIDAD + 1; i++)
		{
			RegistroVariable registro;
			archivo.get_registro_ocupado(&registro,ref_lista+i);

			IDdocumento_t id_doc;
			CantidadPosiciones_t cantidad_posiciones;
			ReferenciaListaPos_t ref_lista_pos;

			registro.recuperar_campo((char*)&id_doc,0);
			registro.recuperar_campo((char*)&cantidad_posiciones,1);
			registro.recuperar_campo((char*)&ref_lista_pos,2);

			ListaInvertida_t lista_invertida;
			lista_invertida.id_doc = id_doc;
			lista_invertida.cantidad_pos = cantidad_posiciones;
			lista_invertida.ref_lista_pos = ref_lista_pos;
			vector_lista_invertida.push_back(lista_invertida);
		}
	}

	return vector_lista_invertida;
}
