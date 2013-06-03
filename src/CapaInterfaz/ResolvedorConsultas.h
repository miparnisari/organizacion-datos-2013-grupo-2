/*
 * ResolvedorConsultas.h
 *
 *  Created on: May 26, 2013
 *      Author: maine
 */

#ifndef RESOLVEDORCONSULTAS_H_
#define RESOLVEDORCONSULTAS_H_

#include "../CapaFisica/ManejadorRegistrosVariables.h"
#include "../CapaFisica/RegistroVariable.h"

#include "../CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../CapaLogica/ArbolBMas/IterArbolBMas.h"
#include "../CapaLogica/Indices/IndiceInvertido.h"

#include "../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../Constantes.h"

class ResolvedorConsultas {
public:
	ResolvedorConsultas(std::string & directorioSalida);
	virtual ~ResolvedorConsultas();

	std::vector<int> get_id_canciones_autor(std::string & autor);
	std::string get_nombre_archivo (int id_cancion);
	std::vector<int> get_ids_canciones_frases(std::string & frase);
	RegistroCancion* get_reg_completo (int id_cancion);
	int get_id_cancion_titulo(std::string & titulo);

private:
	HashingExtensible indicePrimario;
	ManejadorRegistrosVariables archivoMaestro;
	ArbolBMas indiceSecundarioAutor;
	HashingExtensible indiceSecundarioTitulo;
	IndiceInvertido indiceSecundarioFrases;
	HashingExtensible documentos;

	std::string directorioSalida;


};

#endif /* RESOLVEDORCONSULTAS_H_ */
