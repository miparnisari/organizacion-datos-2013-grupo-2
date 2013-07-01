/*
 * OrdenamientoExterno.h
 *
 *  Created on: May 16, 2013
 *      Author: juan
 */

#ifndef ORDENAMIENTOEXTERNO_H_
#define ORDENAMIENTOEXTERNO_H_

using namespace std;
#include <string>
#include <vector>

#include "../Utilitarios/Heap.h"
#include "../CapaFisica/ManejadorRegistrosVariables.h"
#include "Utilitarios.h"

class OrdenamientoExterno {
public:
	OrdenamientoExterno(std::string archAOrdenar);
	virtual ~OrdenamientoExterno();

	void ordenar_archivo();
	/*
	 * Sobreescribe el archivo original
	 * */


	/*SOLO PARA TESTS*****************************************/
	std::vector<string> _getVector()
	{
		return archivosTemporalesAFusionar;
	}

	void _generar_runs();
	/*
	 * Utiliza replacement selection
	 */

	void _merge();
	/*
	 * Fusiona todos los runs generados
	 * */

private:

	unsigned cantDeRuns;
	std::string archAOrdenar;
	std::vector<string> archivosTemporalesAFusionar;

	void _fusionar_2_archivos(string arch1, string arch2, string archFinal, bool temporal);
	/*el parametro temporal indica si es la ultima fusion del ordenamiento o no
	 * TRUE: no lo es     FALSE: si lo es
	 *
	 * Se borran los archivos a unir y se crea el que contiene la union, si ya existe un archivo con esa ruta lo borra
	 * */
};

#endif /* ORDENAMIENTOEXTERNO_H_ */
