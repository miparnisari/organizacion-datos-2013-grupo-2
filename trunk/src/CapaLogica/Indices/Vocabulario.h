/*
 * Vocabulario.h
 *
 *  Created on: Jun 27, 2013
 *      Author: maine
 */

#ifndef VOCABULARIO_H_
#define VOCABULARIO_H_

#include "../../Constantes.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "Definiciones.h"

class Vocabulario {
	private:
		ArbolBMas arbol;
		std::string fileName;

	public:
		Vocabulario();
		virtual ~Vocabulario();

		int crear(std::string nombre_archivo);
		int eliminar ();

		int abrir(std::string nombre_archivo);
		int cerrar ();

		int agregar_termino (const Termino_t & termino, const IDtermino_t id_termino, const ReferenciaListaInvertida_t referencia);

		int actualizar_referencia_lista_invertida(const Termino_t & termino, const ReferenciaListaInvertida_t ref_nueva);

		int buscar_termino (const Termino_t & termino, IDtermino_t & id, ReferenciaListaInvertida_t & referencia);

		int eliminar_termino (const Termino_t & termino);
};

#endif /* VOCABULARIO_H_ */
