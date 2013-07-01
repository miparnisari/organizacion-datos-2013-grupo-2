/*
 * ListaTerminos.h
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#ifndef LISTATERMINOS_H_
#define LISTATERMINOS_H_

#include "Definiciones.h"
#include "../../CapaFisica/ManejadorRegistrosVariables.h"

class ListaTerminos {

	private:
		std::string nombre_archivo;
		ManejadorRegistrosVariables archivo;

	public:
		ListaTerminos();
		virtual ~ListaTerminos();

		int crear(std::string nombre_archivo);
		int eliminar();

		int abrir(std::string nombre_archivo);
		int cerrar();

		IDtermino_t agregar(const Termino_t & termino);

		Termino_t get_termino(const IDtermino_t & id_termino);
};

#endif /* LISTATERMINOS_H_ */
