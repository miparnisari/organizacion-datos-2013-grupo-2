/*
 * ListaPosiciones.h
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#ifndef LISTAPOSICIONES_H_
#define LISTAPOSICIONES_H_

#include "Definiciones.h"
#include "../../CapaFisica/ManejadorArchivoDatosBasicos.h"

class ListaPosiciones {

	private:
		std::string nombre_archivo;
		ManejadorArchivoDatosBasicos<PosicionTermino_t> archivo;		// Registro::(posicion)+

		int _guardar_posicion(const PosicionTermino_t pos);
		PosicionTermino_t _devolver_posicion(const int posicion_guardado);

	public:
		ListaPosiciones();
		virtual ~ListaPosiciones();

		int crear(std::string nombre_archivo);
		int eliminar();

		int abrir(std::string nombre_archivo);
		int cerrar();

		ReferenciaListaPos_t guardar_lista (const ListaPos_t & lista) ;

		ListaPos_t devolver_lista (const CantidadPosiciones_t cuantos, const ReferenciaListaPos_t desde);
};

#endif /* LISTAPOSICIONES_H_ */
