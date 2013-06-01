#include "Indexador.h"


Indexador::Indexador()
{
}


Indexador::~Indexador()
{
	
}

int Indexador::consultar_titulo(std::string & directorioSalida, std::string & titulo)
{
	ResolvedorConsultas rc(directorioSalida);
	int id = rc.get_id_cancion_titulo(titulo);
	if (id != RES_RECORD_DOESNT_EXIST)
	{
		std::cout << "Nombre archivo = " << rc.get_nombre_archivo(id) << "." << std::endl;
	}
	return RES_OK;
}

int Indexador::consultar_autor(std::string & directorioSalida, std::string & autor)
{
	ResolvedorConsultas rc(directorioSalida);
	std::vector <int> ids = rc.get_id_canciones_autor(autor);
	for (unsigned int i = 0; i < ids.size(); i ++)
	{
		std::cout << "Nombre archivo = " << rc.get_nombre_archivo(ids.at(i)) << "." << std::endl;
	}
	return RES_OK;
}

int Indexador::consultar_frase (std::string & directorioSalida, std::string & frase)
{
	ResolvedorConsultas rc(directorioSalida);
	std::vector <int> ids = rc.get_ids_canciones_frases(frase);
	for (unsigned int i = 0; i < ids.size(); i ++)
	{
		std::cout << "Nombre archivo = " << rc.get_nombre_archivo(ids.at(i)) << "." << std::endl;
	}
	return RES_OK;
}

int Indexador::_mostrar_opciones(std::string & directorioEntrada, std::string & directorioSalida)
{
	bool dirEntradaExiste = utilitarios::directorio_existe(directorioEntrada);
	if (! dirEntradaExiste)
	{
		std::cout << "ERROR: El directorio a indexar no existe." << std::endl;
		return RES_DIRECTORY_DOESNT_EXIST;
	}

	bool dirSalidaExiste = utilitarios::directorio_existe(directorioSalida);
	if (! dirSalidaExiste)
	{
		std::cout << "AVISO: El directorio de salida no existe. " << std::endl;
		int resmkdir = mkdir(directorioSalida.c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
		if (resmkdir == -1)
		{
			std::cout << "ERROR: El directorio de salida no se pudo crear."<< std::endl;
			return RES_ERROR;
		}
		else {
			std::cout << "Se creó el directorio de salida." << std::endl;
			return OPCION_INDEXAR;
		}
	}
	else{
		int opcion = 0;
		while (opcion != OPCION_INDEXAR && opcion != OPCION_ANEXAR)
		{
			std::cout << "AVISO: Ya existe un indice en " << directorioSalida << ". ¿Que desea hacer?" << std::endl;
			std::cout << "(1) Borrar el indice e indexar desde cero." << std::endl;
			std::cout << "(2) Anexar las canciones de " << directorioEntrada << " al índice." << std::endl;
			std::cout << "Ingrese opcion: ";
			cin >> opcion;
		}
		if (opcion == OPCION_INDEXAR)
		{
			if (utilitarios::remover_archivos_de_directorio(directorioSalida) == 0)
			{
				return OPCION_INDEXAR;
			}
			else {
				std::cout << "ERROR: No se pudo borrar el indice existente." << std::endl;
				return RES_ERROR;
			}
		}
		else
		{
			return OPCION_ANEXAR;
		}
	}
}

int Indexador::_crear_archivos_indices(std::string & directorioSalida)
{
	int res = RES_OK;
	res += indicePrimario.crear_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.crear_archivo(directorioSalida+'/'+std::string(FILENAME_ARCH_MAESTRO));
	res += documentos.crear_archivo(directorioSalida+'/'+std::string(FILENAME_ID_DOCS));
	res += indiceSecundarioAutor.crear(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_AUTOR));
	res += indiceSecundarioTitulo.crear_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_TITULO));
	res += indiceSecundarioFrases.crear_indice(directorioSalida+'/',std::string(FILENAME_IDX_SECUN_FRASES));
	return res;
}

int Indexador::_abrir_archivos_indices(std::string & directorioEntrada, std::string & directorioSalida)
{
	int res = RES_OK;
	res += indicePrimario.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ARCH_MAESTRO));
	res += documentos.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ID_DOCS));
	res += indiceSecundarioAutor.abrir(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	res += indiceSecundarioTitulo.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_TITULO));
	res += indiceSecundarioFrases.abrir_indice(directorioSalida+'/',std::string(FILENAME_IDX_SECUN_FRASES));
	res += parser.crear(directorioEntrada);
	return res;
}

int Indexador::_finalizar()
{
	int res = indicePrimario.cerrar_archivo();
	res += documentos.cerrar_archivo();
	res += indiceSecundarioAutor.cerrar();
	res += indiceSecundarioTitulo.cerrar_archivo();
	res += indiceSecundarioFrases.cerrar_indice();
	return res;
}

int Indexador::_anexar(std::string & directorioEntrada, std::string & directorioSalida)
{
	ClaveNumerica id(archivoMaestro.get_cantidad_registros());

	RegistroCancion regCancion;
	std::string nombreArchivo;
	while (! parser.fin_directorio())
	{
		int res = parser.obtener_proxima_cancion(regCancion, nombreArchivo);
		if (res != RES_OK)
		{
			std::cout << "No se indexó " << nombreArchivo << " porque no cumple el estandar especificado." << std::endl;
			continue;
		}

		std::vector<std::string> autores;
		for (unsigned int i = 0; i < regCancion.get_cantidad_autores(); i ++)
		{
			autores.push_back(regCancion.get_autor(i));
		}
		std::string titulo = regCancion.get_titulo();
		std::string idioma = regCancion.get_idioma();
		std::vector<int> idsDelAutor;

		// Verifico que no haya ninguna cancion ya indexada
		// que tenga los mismos tres parametros anteriores
		unsigned int i = 0;
		bool cancionEstaRepetida = false;
		while (i < autores.size() && cancionEstaRepetida == false)
		{
			ResolvedorConsultas rc(directorioSalida);
			idsDelAutor = rc.get_id_canciones_autor(autores[i]);

			unsigned int j = 0;
			while (j < idsDelAutor.size() && cancionEstaRepetida == false)
			{
				ClaveX claveID;
				claveID.set_clave(idsDelAutor[j]);
				RegistroClave reg;
				indicePrimario.devolver(claveID,&reg);

				unsigned long offsetAlMaestro;
				reg.recuperar_campo((char*)&offsetAlMaestro,1);

				RegistroCancion regCancionAlmacenada;

				archivoMaestro.get_registro_por_offset(&regCancionAlmacenada,offsetAlMaestro);
				if (regCancionAlmacenada.get_idioma() == idioma && regCancionAlmacenada.get_titulo() == titulo)
				{
					cancionEstaRepetida = true;
				}
				j++;

			}
			i++;
		}

		if (cancionEstaRepetida == true)
		{
			std::cout << "No se indexará la cancion " << titulo << " porque está repetida." << std::endl;
			continue;
		}

		_agregar_a_los_indices(id, regCancion,nombreArchivo);
		id.incrementar();

	}

	return _finalizar();
}

void Indexador::_agregar_a_los_indices (ClaveNumerica & id, RegistroCancion & regCancion, std::string nombreArchivo)
{
	/* ------ guardamos el registro de la cancion en un archivo maestro ------ */
	regCancion.comprimir(compresor);
	long offsetInicialRegCancion = archivoMaestro.agregar_registro(&regCancion);

	/* ------ guardamos el numero de documento (clave) y el documento -------- */

	RegistroClave regDoc;
	ClaveX IDdoc;
	IDdoc.set_clave(id.get_dato());

	regDoc.set_clave(IDdoc);
	regDoc.agregar_campo(nombreArchivo.c_str(),nombreArchivo.size());
	this->documentos.agregar(regDoc);

	/* ----- agregamos al indice primario: ID cancion + offset de la cancion ----*/

	RegistroClave regClave;
	ClaveX clave;
	clave.set_clave(id.get_dato());
	regClave.set_clave(clave);
	regClave.agregar_campo((char*)&offsetInicialRegCancion,sizeof(offsetInicialRegCancion));
	indicePrimario.agregar(regClave);

	/* para cada autor de la cancion:
	 * ----- agregamos al indice por autor: autor + ID cancion ----
	 */

	unsigned cantAutores = regCancion.get_cantidad_autores();
	for (unsigned int i = 0; i < cantAutores; i++)
	{
		RegistroClave regAutorID;

		ClaveX claveAutor;

		/*** En la clave hay "autor"(string) + "id cancion"(string) ***/
		std::string clave = regCancion.get_autor(i);
		std::stringstream idCancion;
		idCancion << id.get_dato();
		clave.append(idCancion.str());

		claveAutor.set_clave(clave);

		regAutorID.set_clave(claveAutor);

		indiceSecundarioAutor.agregar(regAutorID);
	}

	/* ----- agregamos al indice por titulo: el titulo + ID cancion ----*/

	RegistroClave regClave3;
	ClaveX claveTitulo;
	claveTitulo.set_clave(regCancion.get_titulo());
	regClave3.set_clave(claveTitulo);

	int idCancion = id.get_dato();
	regClave3.agregar_campo((char*)&idCancion,sizeof(idCancion));
	indiceSecundarioTitulo.agregar(regClave3);


	/* ----- agregamos al indice por frase: frases ----*/
	std::string letra = regCancion.get_letra();
//	indiceSecundarioFrases.agregar_texto(letra,id.get_dato()); FIXME descomentar

	std::cout << "Se indexó " << nombreArchivo << " correctamente!" << std::endl;
}

void Indexador::_indexar()
{
	ClaveNumerica id(0);

	RegistroCancion regCancion;
	std::string nombreArchivo;

	// Para cada cancion que tengamos...
	while (! parser.fin_directorio())
	{
		int res = parser.obtener_proxima_cancion(regCancion,nombreArchivo);
		if (res != RES_OK)
		{
			std::cout << "No se indexó " << nombreArchivo << " porque no cumple el estándar especificado." << std::endl;
			continue;
		}
		_agregar_a_los_indices(id, regCancion,nombreArchivo);
		id.incrementar();
	}
}

int Indexador::indexar (std::string & directorioEntrada, std::string & directorioSalida)
{
	int opcion = _mostrar_opciones(directorioEntrada,directorioSalida);
	if (opcion == RES_ERROR || opcion == RES_DIRECTORY_DOESNT_EXIST)
		return opcion;

	else
	{
		if (opcion == OPCION_ANEXAR)
		{
			int res2 = _abrir_archivos_indices(directorioEntrada,directorioSalida);
			if (res2 != RES_OK) {
				cout << "ERROR: No se pudieron abrir los archivos necesarios." << endl;
			}
			else {
				_anexar(directorioEntrada,directorioSalida);
			}
		}

		else // OPCION = INDEXAR
		{
			int res2 = _crear_archivos_indices(directorioSalida);
			if (res2 != RES_OK) {
				cout << "ERROR: No se pudieron crear los archivos necesarios." << endl;
			}
			else {
				_abrir_archivos_indices(directorioEntrada,directorioSalida);
				_indexar();
			}
		}

	}
	int res_fin =  _finalizar();
	if (res_fin != RES_OK)
		std::cout << "ERROR: No se pudieron cerrar los archivos.";
	return RES_OK;
}

