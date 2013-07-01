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
#include "../CapaLogica/Indices/IndiceInvertidoBooleanoConPosicion.h"
#include "../CapaLogica/Indices/IndicePorTitulo.h"

#include "../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../Constantes.h"
#include "../CapaLogica/Compresor/PPMC.h"
#include "../CapaLogica/Indices/Definiciones.h"

class ResolvedorConsultas {
public:
	ResolvedorConsultas();
	virtual ~ResolvedorConsultas();

	void set_directorio_indice(std::string & dirSalida);

	std::vector<IDdocumento_t> get_id_canciones_autor(std::string & autor);

	std::vector<IDdocumento_t> get_ids_canciones_frases(std::string frase);

	std::vector<IDdocumento_t> get_ids_cancion_titulo(std::string & titulo);

	std::string get_nombre_archivo (IDdocumento_t id_cancion);
	RegistroCancion* get_reg_completo (IDdocumento_t id_cancion);


private:
	HashingExtensible indicePrimario;
	ManejadorRegistrosVariables archivoMaestro;
	ArbolBMas indiceSecundarioAutor;
	IndicePorTitulo indiceSecundarioTitulo;
	IndiceInvertidoBooleanoConPosicion indiceSecundarioFrases;
	HashingExtensible documentos;

	std::string directorioSalida;

};

#endif /* RESOLVEDORCONSULTAS_H_ */
