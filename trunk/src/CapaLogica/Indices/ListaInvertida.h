/*
 * ListaInvertida.h
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#ifndef LISTAINVERTIDA_H_
#define LISTAINVERTIDA_H_

#include "Definiciones.h"
#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include <vector>

class ListaInvertida {


	private:
		std::string nombre_archivo;
		ManejadorRegistrosVariables archivo;		// Registro::(IDDocumento,cantidadPosiciones,refListaPos)

	public:
		ListaInvertida();
		virtual ~ListaInvertida();

		int crear(std::string nombre_archivo);
		int eliminar();

		int abrir(std::string nombre_archivo);
		int cerrar();

		ReferenciaListaInvertida_t guardar_listas (const std::vector<ListaInvertida_t> & listas);

		std::vector<ListaInvertida_t> recuperar_listas (const ReferenciaListaInvertida_t ref_lista);
};

#endif /* LISTAINVERTIDA_H_ */
