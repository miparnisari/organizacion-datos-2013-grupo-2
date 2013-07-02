/*
 * TablaOrdenes.h
 *
 *  Created on: Jul 2, 2013
 *      Author: maine
 */

#ifndef TABLAORDENES_H_
#define TABLAORDENES_H_

#include "TablaModelos.h"
#include "ModeloProbabilistico.h"
#include "../../Utilitarios/Logger.h"

#include <map>

class TablaOrdenes {

	private:
		int cantidad_ordenes;
		std::map<int,TablaModelos*> mapa_ordenes;

		void _inicializar_orden_menosuno();
		void _inicializar_orden_cero();

	public:
		TablaOrdenes(int cantidad_ordenes);
		virtual ~TablaOrdenes();

		void inicializar();

		void limpiar_ordenes();

		void imprimir();

		void crear_modelo (int orden, string nombre_modelo);
		void actualizar_modelo(int orden, Uint simbolo, string contexto_del_simbolo);
		ModeloProbabilistico* devolver_modelo(int orden, string& contexto);
};

#endif /* TABLAORDENES_H_ */
