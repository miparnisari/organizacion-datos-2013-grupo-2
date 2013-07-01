/*
 * Definiciones.h
 *
 *  Created on: Jun 28, 2013
 *      Author: maine
 */

#ifndef DEFINICIONES_H_
#define DEFINICIONES_H_

#include <string>
#include <vector>

const int					REFERENCIA_INVALIDA = -1;

/*
 * Definiciones de tipos
 */
typedef std::string 							Documento_t;
typedef unsigned int 							IDdocumento_t;

typedef std::string 							Termino_t;
typedef unsigned int 							IDtermino_t;

typedef unsigned int							PosicionTermino_t;
typedef unsigned int							CantidadPosiciones_t;

typedef int 									Referencia_t;
typedef Referencia_t							ReferenciaListaPos_t;
typedef Referencia_t							ReferenciaListaInvertida_t;

typedef std::vector<PosicionTermino_t> 		ListaPos_t;

typedef struct _ListaInvertida_t
{
	IDdocumento_t id_doc;
	CantidadPosiciones_t cantidad_pos;
	ReferenciaListaPos_t ref_lista_pos;
}ListaInvertida_t;

typedef struct _ListaPosicionesTerminosEnDocumentos_t
{
	IDtermino_t id_termino;
	IDdocumento_t id_doc;
	PosicionTermino_t pos_termino;
}ListaPosicionesTerminosEnDocumentos_t;

/*
 * Nombres de archivos
 *
 */
const std::string FILENAME_LISTAS_POS[]			= "ListasPosiciones.dat";
const std::string FILENAME_LISTAS_INV[]			= "ListasInvertidas.dat";
const std::string FILENAME_VOCABULARIO[]		= "Vocabulario.dat";
const std::string FILENAME_TERMINOS[]			= "Terminos.dat";



#endif /* DEFINICIONES_H_ */
