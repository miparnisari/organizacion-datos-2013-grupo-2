/*
 * ListaPosicionesTerminosEnDocumentos.h
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#ifndef LISTAPOSICIONESTERMINOSENDOCUMENTOS_H_
#define LISTAPOSICIONESTERMINOSENDOCUMENTOS_H_

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../Utilitarios/OrdenamientoExterno.h"
#include "Definiciones.h"
#include "../../Utilitarios/Logger.h"
#include "../../Utilitarios/Utilitarios.h"
#include <cassert>

class ListaPosicionesTerminosEnDocumentos {

	private:
		std::string nombre_archivo;
		ManejadorRegistrosVariables archivo;		// Registro::(IDtermino, IDdocumento, posicion)

		void _comparar_registros();


	public:
		ListaPosicionesTerminosEnDocumentos();
		virtual ~ListaPosicionesTerminosEnDocumentos();

		int crear(std::string nombre_archivo);
		int eliminar();

		int abrir(std::string nombre_archivo);
		int cerrar();

		int ordenar_por_ID_termino();

		int agregar(const ListaPosicionesTerminosEnDocumentos_t & lista);

		int devolver (int posicion, ListaPosicionesTerminosEnDocumentos_t & lista );

};

#endif /* LISTAPOSICIONESTERMINOSENDOCUMENTOS_H_ */
