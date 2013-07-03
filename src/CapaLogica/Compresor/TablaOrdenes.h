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
#include <cassert>

class TablaOrdenes {

	private:
		int cantidad_ordenes;
		ModeloProbabilistico* modelo_cero;
		ModeloProbabilistico* modelo_menos_uno;
		std::map<int,TablaModelos*> resto_modelos;

		void _inicializar_orden_menosuno();
		void _inicializar_orden_cero();
		void inicializar();
		void limpiar_ordenes();

	public:
		TablaOrdenes(int cantidad_ordenes);
		virtual ~TablaOrdenes();


		void imprimir();

		void crear_modelo (int orden, string nombre_modelo);
		void actualizar_modelo(int orden, Uint simbolo, string contexto_del_simbolo);
		ModeloProbabilistico* devolver_modelo(int orden, string& contexto);
};

#endif /* TABLAORDENES_H_ */
