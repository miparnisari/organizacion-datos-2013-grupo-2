/*
 * IndiceInvertidoBooleanoConPosicion.h
 *
 *  Created on: Jun 27, 2013
 *      Author: maine
 */

#ifndef INDICEINVERTIDOBOOLEANOCONPOSICION_H_
#define INDICEINVERTIDOBOOLEANOCONPOSICION_H_

#include <string>
#include <algorithm>
#include "../../Constantes.h"
#include "../../Utilitarios/Logger.h"
#include "../../Utilitarios/Utilitarios.h"

#include "../../CapaLogica/ManejoArchivos/Texto.h"
#include "Definiciones.h"
#include "Vocabulario.h"
#include "ListaInvertida.h"
#include "ListaPosiciones.h"
#include "ListaPosicionesTerminosEnDocumentos.h"
#include "ListaTerminos.h"

class IndiceInvertidoBooleanoConPosicion {

	public:
		IndiceInvertidoBooleanoConPosicion();
		virtual ~IndiceInvertidoBooleanoConPosicion();

		int crear(std::string nombre_archivo);
		int eliminar();

		int abrir(std::string nombre_archivo);
		int cerrar();

		int agregar(const Documento_t & doc, const IDdocumento_t id_doc);
		int finalizar_agregado();

		int eliminar(const Documento_t & doc, const IDdocumento_t id_doc);
		int buscar_frase(const string & frase, vector<IDdocumento_t> & id_docs);

	private:
		std::string fileName;
		Vocabulario vocabulario; 		// Registro::(i(termino), IDtermino, refListaInvertida)
		ListaInvertida listasInvertidas;	// Registro::(IDDocumento,cantidad,refListaPos)
		ListaTerminos listasTerminos;		// Registro::(termino)
		ListaPosiciones listasPosicion;		// Registro::(posicion)+
		ListaPosicionesTerminosEnDocumentos posicionesDeTerminosEnDocumentos; 	// Registro::(IDtermino, IDdocumento, posicion)

		ReferenciaListaInvertida_t _guardar_lista_invertida(const IDtermino_t id_termino, const std::vector<std::pair<IDdocumento_t,PosicionTermino_t> > & v_IDdoc_posTermino);

		int _buscar_termino(const Termino_t & termino, std::vector<std::pair<IDdocumento_t,PosicionTermino_t> > & v_IDdocs_posiciones);

		vector<pair<IDdocumento_t,PosicionTermino_t> > _interseccion_dos_listas(const vector<pair<IDdocumento_t,PosicionTermino_t> > & v1, const vector<pair<IDdocumento_t,PosicionTermino_t> > & v2);

};

#endif /* INDICEINVERTIDOBOOLEANOCONPOSICION_H_ */
