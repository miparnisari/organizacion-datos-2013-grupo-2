#include "Indexador.h"


Indexador::Indexador()
{
}


Indexador::~Indexador()
{
	
}

int Indexador::borrar_cancion (std::string & directorioSalida, int idCancion, RegistroCancion & reg)
{
	_abrir_archivos_indices(directorioSalida);

	ClaveX claveID, claveAutor, claveTitulo;

	int offsetArchivoMaestro;
	RegistroClave regClave;
	claveID.set_clave(idCancion);
	if (indicePrimario.devolver(claveID,&regClave) == RES_RECORD_DOESNT_EXIST)
	{
		std::cout << "No se pudo eliminar la canción con ID " << idCancion << "." << std::endl;
		return RES_ERROR;
	}
	else {
		regClave.recuperar_campo((char*)&offsetArchivoMaestro,1);

		indicePrimario.eliminar(claveID);


		archivoMaestro.eliminar_registro_por_offset(offsetArchivoMaestro);

		RegistroClave regArbol;
		for (unsigned int i = 0; i < reg.get_cantidad_autores(); i++){
			std::string clave = reg.get_autor(i);
			clave.append(utilitarios::int_a_string(idCancion));

			claveAutor.set_clave(clave);

			regArbol.set_clave(claveAutor);
			indiceSecundarioAutor.quitar(regArbol);
		}

		claveTitulo.set_clave(reg.get_titulo());
		indiceSecundarioTitulo.eliminar(claveTitulo);


		// TODO eliminar del indiceSecundarioFrases;

		documentos.eliminar(claveID);
	}

	return _finalizar();
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

int Indexador::_abrir_archivos_indices(std::string & directorioSalida)
{
	int res = RES_OK;
	res += indicePrimario.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ARCH_MAESTRO));
	res += documentos.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_ID_DOCS));
	res += indiceSecundarioAutor.abrir(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	res += indiceSecundarioTitulo.abrir_archivo(directorioSalida+'/'+std::string(FILENAME_IDX_SECUN_TITULO));
	res += indiceSecundarioFrases.abrir_indice(directorioSalida+'/',std::string(FILENAME_IDX_SECUN_FRASES));
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

int Indexador::_anexar(std::string & directorioEntrada, std::string & directorioSalida, ResolvedorConsultas & rc)
{
	ClaveNumerica id(archivoMaestro.get_cantidad_registros());

	RegistroCancion regCancionNormalizado;
	RegistroCancion regCancionNoNormalizado;
	std::string nombreArchivo;
	while (! parser.fin_directorio())
	{
		int res = parser.obtener_proxima_cancion(regCancionNormalizado, regCancionNoNormalizado, nombreArchivo);
		if (res != RES_OK)
		{
			std::cout << "No se indexó " << nombreArchivo << " porque no cumple el estandar especificado." << std::endl;
			continue;
		}

		std::vector<std::string> autores;
		for (unsigned int i = 0; i < regCancionNormalizado.get_cantidad_autores(); i ++)
		{
			autores.push_back(regCancionNormalizado.get_autor(i));
		}
		std::string titulo = regCancionNormalizado.get_titulo();
		std::string idioma = regCancionNormalizado.get_idioma();
		std::vector<int> idsDelAutor;

		// Verifico que no haya ninguna cancion ya indexada
		// que tenga los mismos tres parametros anteriores
		unsigned int i = 0;
		bool cancionEstaRepetida = false;
		while (i < autores.size() && cancionEstaRepetida == false)
		{
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

		_agregar_a_los_indices(id, regCancionNormalizado, regCancionNoNormalizado, nombreArchivo);
		id.incrementar();

	}

	return _finalizar();
}

void Indexador::_agregar_a_los_indices (
		ClaveNumerica & id,
		RegistroCancion & regCancion,
		RegistroCancion & regCancionNoNormalizada,
		std::string nombreArchivo)
{
	/* ------ guardamos el registro de la cancion en un archivo maestro ------ */
	RegistroVariable* regComprimido = regCancionNoNormalizada.comprimir(compresor);
	long offsetInicialRegCancion = archivoMaestro.agregar_registro(regComprimido);
	delete regComprimido;

	/* ------ guardamos el numero de documento (clave) y el documento -------- */

	RegistroClave regDoc;
	ClaveX* IDdoc = new ClaveX();
	IDdoc->set_clave(id.get_dato());
	regDoc.set_clave(*IDdoc);

	regDoc.agregar_campo(nombreArchivo.c_str(),nombreArchivo.size());
	this->documentos.agregar(regDoc);

	/* ----- agregamos al indice primario: ID cancion + offset de la cancion ----*/

	RegistroClave regClave;
	regClave.set_clave(*IDdoc);
	delete IDdoc;
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
		clave.append(utilitarios::int_a_string(id.get_dato()));

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
	indiceSecundarioFrases.agregar_texto(letra,id.get_dato());

	std::cout << "Se indexó " << nombreArchivo << " correctamente!" << std::endl;
}

void Indexador::_indexar()
{
	ClaveNumerica id(0);

	RegistroCancion regCancionNormalizada;
	RegistroCancion regCancionNoNormalizada;
	std::string nombreArchivo;

	// Para cada cancion que tengamos...
	while (! parser.fin_directorio())
	{
		int res = parser.obtener_proxima_cancion(regCancionNormalizada,regCancionNoNormalizada,nombreArchivo);
		if (res != RES_OK)
		{
			std::cout << "No se indexó " << nombreArchivo << " porque no cumple el estándar especificado." << std::endl;
		}
		else {
			_agregar_a_los_indices(id, regCancionNormalizada, regCancionNoNormalizada, nombreArchivo);
			id.incrementar();
		}
	}
}

int Indexador::indexar (std::string & directorioEntrada, std::string & directorioSalida, ResolvedorConsultas& rc)
{
	int opcion = _mostrar_opciones(directorioEntrada,directorioSalida);
	if (opcion == RES_ERROR || opcion == RES_DIRECTORY_DOESNT_EXIST)
		return opcion;

	else
	{
		parser.crear(directorioEntrada);
		if (opcion == OPCION_ANEXAR)
		{
			int res2 = _abrir_archivos_indices(directorioSalida);
			if (res2 != RES_OK) {
				cout << "ERROR: No se pudieron abrir los archivos necesarios." << endl;
			}
			else {
				_anexar(directorioEntrada,directorioSalida, rc);
			}
		}

		else // OPCION = INDEXAR
		{
			int res2 = _crear_archivos_indices(directorioSalida);
			if (res2 != RES_OK) {
				cout << "ERROR: No se pudieron crear los archivos necesarios." << endl;
			}
			else {
				_abrir_archivos_indices(directorioSalida);
				_indexar();
			}
		}

	}
	int res_fin =  _finalizar();
	if (res_fin != RES_OK)
		std::cout << "ERROR: No se pudieron cerrar los archivos.";
	return RES_OK;
}
