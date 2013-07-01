/*
 * IndiceInvertidoBooleanoConPosicion.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: maine
 */

#include "IndiceInvertidoBooleanoConPosicion.h"

IndiceInvertidoBooleanoConPosicion::IndiceInvertidoBooleanoConPosicion() {

}

IndiceInvertidoBooleanoConPosicion::~IndiceInvertidoBooleanoConPosicion() {
}

int IndiceInvertidoBooleanoConPosicion::crear(std::string nombre_archivo)
{
	fileName = nombre_archivo;
	int res = vocabulario.crear(fileName+"Vocabulario.dat");
	res += listasInvertidas.crear(fileName+"ListasInvertidas.dat");
	res += listasTerminos.crear(fileName+"ListasTerminos.dat");
	res += listasPosicion.crear(fileName+"ListasPosicion.dat");
	res += posicionesDeTerminosEnDocumentos.crear(fileName+"PosicionesTerminosEnDocs.dat");
	return res;
}
int IndiceInvertidoBooleanoConPosicion::eliminar()
{
	int res = vocabulario.eliminar();
	res += listasInvertidas.eliminar();
	res += listasTerminos.eliminar();
	res += listasPosicion.eliminar();
	res += posicionesDeTerminosEnDocumentos.eliminar();
	return res;
}

int IndiceInvertidoBooleanoConPosicion::abrir(std::string nombre_archivo)
{
	fileName = nombre_archivo;
	int res = vocabulario.abrir(fileName+"Vocabulario.dat");
	res += listasInvertidas.abrir(fileName+"ListasInvertidas.dat");
	res += listasTerminos.abrir(fileName+"ListasTerminos.dat");
	res += listasPosicion.abrir(fileName+"ListasPosicion.dat");
	res += posicionesDeTerminosEnDocumentos.abrir(fileName+"PosicionesTerminosEnDocs.dat");
	return res;
}
int IndiceInvertidoBooleanoConPosicion::cerrar()
{
	int res = vocabulario.cerrar();
	res += listasInvertidas.cerrar();
	res += listasTerminos.cerrar();
	res += listasPosicion.cerrar();
	res += posicionesDeTerminosEnDocumentos.cerrar();
	return res;
}

int IndiceInvertidoBooleanoConPosicion::agregar(const Documento_t & doc, const IDdocumento_t id_doc)
{
	/*
	 * Por cada término relevante t (formato alfabético) encontrado en un documento idD en la posición relativa
dentro del documento p:
		a. Se busca t en el vocabulario (archivo B# o diccionario en RAM).
			Si está:
			 	 se obtiene su idT numérico;
			Si no:
				se registra t al final del archivo de términos por orden de aparición, con
				su posición relativa determinando su idT,
				y se agrega (t, idT, null) al vocabulario.
		b. Se registra (idT, idD, p) en el archivo de posiciones de términos en documentos.
	 *
	 */
	TextoNormalizado doc_parseado;
	doc_parseado.parsear(doc);

	Termino_t termino;
	IDtermino_t id_termino = 0;
	PosicionTermino_t posTermino = 0;
	ReferenciaListaInvertida_t referenciaListaInvertida;

	while (doc_parseado.get_proxima_palabra(termino) != RES_FIN)
	{
		int resultado_busqueda = vocabulario.buscar_termino(termino,id_termino,referenciaListaInvertida);
		bool termino_esta_en_voc = (resultado_busqueda == RES_OK);
		if (termino_esta_en_voc)
		{
			// Ya tenemos el id_termino.
		}
		else
		{
			id_termino = listasTerminos.agregar(termino);
			vocabulario.agregar_termino (termino, id_termino, REFERENCIA_INVALIDA);
		}

		/*
		 * Se registra (idT, idD, p) en el archivo de posiciones de términos en documentos.
		 */
		ListaPosicionesTerminosEnDocumentos_t lista;
		lista.id_doc = id_doc;
		lista.id_termino = id_termino;
		lista.pos_termino = posTermino;
		posicionesDeTerminosEnDocumentos.agregar(lista);
		posTermino ++;
	}

	return RES_OK;
}

int IndiceInvertidoBooleanoConPosicion::finalizar_agregado()
{
	/*
	 *
				Recorriendo secuencialmente este archivo una vez, se arman las listas de posiciones de términos en
				documentos


	para cada IDtermino:
		vector nuevo
		agregar al vector <IDdocumento, posicion>
		para cada IDdocumento:
			vector de posiciones nuevo
			para cada posicion:
				agregar posicion al vector de posiciones

			ref_lista = agregar vector de posiciones al archivo de posiciones

			agregar <IDdocumento,ref_lista> al archivo de lista invertida

	 *
	 */

	posicionesDeTerminosEnDocumentos.ordenar_por_ID_termino();

	IDdocumento_t id_documento;
	Termino_t termino;
	IDtermino_t id_termino_actual = 0;
	PosicionTermino_t pos_termino;

	std::vector<pair<IDdocumento_t,PosicionTermino_t>> vector_IDdoc_posTermino;

	int num_tupla = 0;

	ListaPosicionesTerminosEnDocumentos_t tupla;
	int res = posicionesDeTerminosEnDocumentos.devolver(num_tupla, tupla);

	id_termino_actual = 0;

	while (res != RES_ERROR)
	{

		// Para un ID termino, busco en que documentos y en que posiciones aparece
		while ((id_termino_actual == tupla.id_termino) && (res != RES_ERROR))
		{
			id_documento = tupla.id_doc;
			pos_termino = tupla.pos_termino;
			vector_IDdoc_posTermino.push_back( make_pair(id_documento,pos_termino));

			num_tupla ++;
			res = posicionesDeTerminosEnDocumentos.devolver(num_tupla, tupla);

		}


		ReferenciaListaInvertida_t ref_lista_invertida = _guardar_lista_invertida(id_termino_actual,vector_IDdoc_posTermino);

		termino = listasTerminos.devolver(id_termino_actual);

		vocabulario.actualizar_referencia_lista_invertida(termino,ref_lista_invertida);

		id_termino_actual = tupla.id_termino;
		vector_IDdoc_posTermino.clear();

	}

	return RES_OK;
}

ReferenciaListaInvertida_t IndiceInvertidoBooleanoConPosicion::_guardar_lista_invertida
		(const IDtermino_t id_termino,
		const std::vector<std::pair<IDdocumento_t,PosicionTermino_t> > & v_IDdoc_posTermino)
{
	const unsigned int CANTIDAD_OCURRENCIAS_TERMINO = v_IDdoc_posTermino.size();

	std::vector<PosicionTermino_t> v_posiciones_un_id_doc;

	unsigned int i = 0;
	PosicionTermino_t pos;
	IDdocumento_t id_doc_actual = v_IDdoc_posTermino[0].first;

	vector<ListaInvertida_t> vector_listas_invertidas;
	ReferenciaListaInvertida_t ref_lista_inv;


	while (i < CANTIDAD_OCURRENCIAS_TERMINO)
	{

		while (i < CANTIDAD_OCURRENCIAS_TERMINO && v_IDdoc_posTermino[i].first == id_doc_actual)
		{
			pos	= v_IDdoc_posTermino[i].second;
			v_posiciones_un_id_doc.push_back(pos);

			i ++;
		}

		sort(v_posiciones_un_id_doc.begin(),v_posiciones_un_id_doc.end());
		ReferenciaListaPos_t ref_lista_pos = listasPosicion.guardar(v_posiciones_un_id_doc);
		CantidadPosiciones_t cantidad_posiciones = v_posiciones_un_id_doc.size();
		ListaInvertida_t lista_invertida;
		lista_invertida.id_doc = id_doc_actual;
		lista_invertida.cantidad_pos = cantidad_posiciones;
		lista_invertida.ref_lista_pos = ref_lista_pos;

		Termino_t termino = listasTerminos.devolver(id_termino);

		vector_listas_invertidas.push_back(lista_invertida);

		v_posiciones_un_id_doc.clear();
		if (i < CANTIDAD_OCURRENCIAS_TERMINO)
			id_doc_actual = v_IDdoc_posTermino[i].first;
	}

	ref_lista_inv = listasInvertidas.agregar(vector_listas_invertidas);
	return ref_lista_inv;

}

int IndiceInvertidoBooleanoConPosicion::eliminar(const Documento_t & doc, const IDdocumento_t id_doc)
{
	//TODO
	return RES_OK;
}

int IndiceInvertidoBooleanoConPosicion::_buscar_termino(const Termino_t & termino, vector<pair<IDdocumento_t,PosicionTermino_t>> & v_IDdocs_posiciones)
{
	v_IDdocs_posiciones.clear();
	IDtermino_t id_termino;
	ReferenciaListaInvertida_t ref_lista_invertida = REFERENCIA_INVALIDA;


	int res_busqueda = vocabulario.buscar_termino(termino, id_termino, ref_lista_invertida);
	if (res_busqueda == RES_OK)
	{
		std::vector<ListaInvertida_t> vectorListasInvertidas = listasInvertidas.devolver(ref_lista_invertida);

		const int CANTIDAD_LISTAS_INVERTIDAS = vectorListasInvertidas.size();

		for (int i = 0; i < CANTIDAD_LISTAS_INVERTIDAS; i ++)
		{
			ListaInvertida_t lista_invertida = vectorListasInvertidas[i];

			IDdocumento_t id_documento = lista_invertida.id_doc;
			CantidadPosiciones_t cantidad_posiciones = lista_invertida.cantidad_pos;
			ReferenciaListaPos_t ref_lista_pos = lista_invertida.ref_lista_pos;

			ListaPos_t lista_posiciones = listasPosicion.devolver(cantidad_posiciones,ref_lista_pos);

			for (unsigned int j = 0; j < lista_posiciones.size(); j ++)
			{
				PosicionTermino_t pos = lista_posiciones[j];
				v_IDdocs_posiciones.push_back(std::make_pair(id_documento,pos));
			}

		}
	}

	return RES_OK;
}

vector<pair<IDdocumento_t,PosicionTermino_t> > IndiceInvertidoBooleanoConPosicion::_interseccion_dos_listas
	(const vector<pair<IDdocumento_t,PosicionTermino_t> > & v1,
	const vector<pair<IDdocumento_t,PosicionTermino_t> > & v2)
{
	vector<pair<IDdocumento_t,PosicionTermino_t> > interseccion_dos_terminos;

	IDdocumento_t ID_doc_uno, ID_doc_dos;

	int i = 0;
	int j = 0;
	const int MAX_INDICE_V1 = v1.size() -1;
	const int MAX_INDICE_V2 = v2.size() -1;

	while(i <= MAX_INDICE_V1  && j <= MAX_INDICE_V2)
	{
		ID_doc_uno = v1[i].first;
		ID_doc_dos = v2[j].first;
		if (ID_doc_uno < ID_doc_dos)
			i++;
		else if (ID_doc_uno > ID_doc_dos)
			j++;
		else
		{
			PosicionTermino_t pos_uno = v1[i].second;
			PosicionTermino_t pos_dos = v2[j].second;

			// Hubo match de posicion
			if (pos_uno + 1 == pos_dos)
			{
				interseccion_dos_terminos.push_back(make_pair(ID_doc_uno,pos_dos));
				// Me muevo en ambas listas
				i++;
				j++;
			}

			// No hubo match de posicion
			else
			{
				if (pos_uno < pos_dos)
					i++;
				else
					j++;
			}

		}
	}

	return interseccion_dos_terminos;
}

int IndiceInvertidoBooleanoConPosicion::buscar_frase(const string & frase, vector<IDdocumento_t> & id_docs)
{
	Termino_t termino;
	id_docs.clear();
	TextoNormalizado frase_parseada;
	frase_parseada.parsear(frase);

	vector<pair<IDdocumento_t,PosicionTermino_t> > vector_termino_dos;
	vector<pair<IDdocumento_t,PosicionTermino_t> > interseccion;

	while (frase_parseada.get_proxima_palabra(termino) != RES_FIN)
	{
		_buscar_termino(termino,interseccion);

		while (frase_parseada.get_proxima_palabra(termino) != RES_FIN)
		{
			_buscar_termino(termino,vector_termino_dos);

			interseccion = _interseccion_dos_listas(interseccion,vector_termino_dos);

		}
	}


	// Elimino los ID docs repetidos
	for (unsigned int i = 0; i < interseccion.size(); i ++)
	{
		IDdocumento_t id_doc = interseccion[i].first;
		if(std::find(id_docs.begin(), id_docs.end(), id_doc) == id_docs.end())
			id_docs.push_back(id_doc);
	}

	return RES_OK;
}
