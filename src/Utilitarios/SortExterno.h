/*
 * SortExterno.h
 *
 *  Created on: May 16, 2013
 *      Author: juan
 */

#ifndef SORTEXTERNO_H_
#define SORTEXTERNO_H_

using namespace std;
#include <string>
#include<vector>


#include "../Utilitarios/Heap.h"
#include "../CapaFisica/ManejadorRegistrosVariables.h"


class SortExterno {
public:
	SortExterno(std::string archAOrdenar);
	virtual ~SortExterno();

	void ordenar_archivo(string nombreArchivo);
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
};

#endif /* SORTEXTERNO_H_ */
