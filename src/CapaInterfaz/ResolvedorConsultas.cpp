/*
 * ResolvedorConsultas.cpp
 *
 *  Created on: May 26, 2013
 *      Author: maine
 */

#include "ResolvedorConsultas.h"

ResolvedorConsultas::ResolvedorConsultas()
{

}
ResolvedorConsultas::~ResolvedorConsultas()
{

}

void ResolvedorConsultas::set_directorio_indice(std::string & dirSalida)
{
	this->directorioSalida = dirSalida;
}

std::vector<int> ResolvedorConsultas::get_ids_canciones_frases(std::string & frase)
{
	std::vector<int> ids;
	RegistroVariable listaInvertida;
	indiceSecundarioFrases.abrir_indice(directorioSalida+'/',std::string(FILENAME_IDX_SECUN_FRASES));

	int res = indiceSecundarioFrases.buscar_frase(frase,listaInvertida);
	if (res != RES_OK || listaInvertida.get_cantidad_campos() == 0)
	{
		std::cout << "La frase " << frase << " no fue encontrada." << std::endl;
	}
	else {

		// Cada campo de la lista invertida tiene un ID de cancion
		unsigned int q = listaInvertida.get_cantidad_campos();
		for (unsigned int i = 0; i < q; i++)
		{
			int iDdoc;
			listaInvertida.recuperar_campo((char*)&iDdoc,i);
			ids.push_back(iDdoc);
		}
	}

	indiceSecundarioFrases.cerrar_indice();
	return ids;
}

std::vector<int> ResolvedorConsultas::get_id_canciones_autor(std::string & autor)
{
	std::vector<int> ids;

	indiceSecundarioAutor.abrir(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	IterArbolBMas buscador(indiceSecundarioAutor);
	ClaveX claveInicio;
	claveInicio.set_clave(autor + (char)0);
	int res = buscador.comienzo(">=",claveInicio);
	if (res == RES_ERROR)
	{
		std::cout << "El autor " << autor << " no fue encontrado." << std::endl;
	}
	else {
		ClaveX claveFin;
		std::string autorSiguiente = autor;
		autorSiguiente[autorSiguiente.size()-1] = (char)(autor[autor.size()-1] + 1);
		claveFin.set_clave(autorSiguiente);

		RegistroClave otraCancion;
		while (buscador.leer_siguiente(otraCancion) != RES_FIN && otraCancion.get_clave() < claveFin)
		{
			ClaveX clave = otraCancion.get_clave();
			std::string autorMasId;
			clave.get_clave(autorMasId);

			std::string soloID;
			soloID.append(autorMasId.begin()+autor.size(), autorMasId.end());
			std::istringstream ss(soloID);
			int idDoc;
			ss >> idDoc;

			ids.push_back(idDoc);
		}

	}

	indiceSecundarioAutor.cerrar();
	return ids;

}
int ResolvedorConsultas::get_id_cancion_titulo(std::string & titulo)
{
	int idDoc;
	indiceSecundarioTitulo.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_TITULO));
	ClaveX claveTitulo;
	claveTitulo.set_clave(titulo);

	RegistroClave regDevuelto;
	int res = indiceSecundarioTitulo.devolver(claveTitulo,&regDevuelto);
	if (res == RES_RECORD_DOESNT_EXIST)
	{
		std::cout << "La cancion " << titulo << " no fue encontrada." << std::endl;
		indiceSecundarioTitulo.cerrar_archivo();
		return RES_RECORD_DOESNT_EXIST;
	}
	else {
		regDevuelto.recuperar_campo((char*)&idDoc,1);
	}

	indiceSecundarioTitulo.cerrar_archivo();
	return idDoc;
}

RegistroCancion* ResolvedorConsultas::get_reg_completo (int id_cancion)
{
	indicePrimario.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_PRIM));
	std::string letra = "";

	ClaveX claveID;
	claveID.set_clave(id_cancion);

	RegistroClave reg;
	indicePrimario.devolver(claveID, &reg);
	indicePrimario.cerrar_archivo();

	archivoMaestro.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ARCH_MAESTRO));

	long offset;
	reg.recuperar_campo((char*)&offset,1);

	Aritmetico* compresor = new Aritmetico();
	RegistroVariable* regVariableComprimido = new RegistroVariable();
	archivoMaestro.get_registro_por_offset(regVariableComprimido,offset);

	RegistroCancion* regDescomprimido = new RegistroCancion();
	regVariableComprimido->descomprimir(compresor, regDescomprimido);

	delete compresor;
	delete regVariableComprimido;
	return regDescomprimido;
}

std::string ResolvedorConsultas::get_nombre_archivo (int id_cancion)
{
	documentos.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ID_DOCS));

	RegistroClave regDoc;
	ClaveX IDdoc;
	IDdoc.set_clave(id_cancion);

	this->documentos.devolver(IDdoc, &regDoc);

	char* bufferNombreArchivo = new char[regDoc.get_tamanio_campo(1) + 1]();
	bufferNombreArchivo [regDoc.get_tamanio_campo(1)] = '\0';
	regDoc.recuperar_campo(bufferNombreArchivo,1);

	documentos.cerrar_archivo();

	std::string nombreArchivo (bufferNombreArchivo);
	delete[] bufferNombreArchivo;

	return nombreArchivo;
}

