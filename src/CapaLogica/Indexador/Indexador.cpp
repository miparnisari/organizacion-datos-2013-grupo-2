#include "Indexador.h"


Indexador::Indexador()
{
}


Indexador::~Indexador()
{
	
}

int Indexador::consultar_autor(std::string & directorioSalida, std::string & unAutor)
{
	indiceSecundarioAutor.abrir(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");

	documentos.abrir_archivo(directorioSalida+std::string(FILENAME_ID_DOCS));
	IterArbolBMas buscador(indiceSecundarioAutor);
	ClaveX claveInicio;
	claveInicio.set_clave(unAutor + (char)0);
	int res = buscador.start(">=",claveInicio);
	if (res == RES_ERROR)
	{
		std::cout << "no se pudo comenzar la busqueda" << std::endl;
	}

	ClaveX claveFin;
	std::string autorSiguiente = unAutor;
	autorSiguiente[autorSiguiente.size()-1] = (char)(unAutor[unAutor.size()-1] + 1);
	claveFin.set_clave(autorSiguiente);

	RegistroClave otraCancion;
	while (buscador.readNext(otraCancion) != RES_FIN && otraCancion.get_clave() < claveFin)
	{
		ClaveX clave = otraCancion.get_clave();

		std::string autorMasId;
		clave.get_clave(autorMasId);

		std::string soloID;
		soloID.append(autorMasId.begin()+unAutor.size(), autorMasId.end());
		std::istringstream ss(soloID);
		int idDoc;
		ss >> idDoc;

		ClaveX claveIDdoc;

		claveIDdoc.set_clave(idDoc);

		RegistroClave regDocumento;
		documentos.devolver(claveIDdoc,&regDocumento);

		char* nombreCancion = new char[regDocumento.get_tamanio_campo(1) +1]();
		nombreCancion[regDocumento.get_tamanio_campo(1)] = '\0';
		regDocumento.recuperar_campo(nombreCancion,1);

		std::cout << "Nombre de cancion = " << nombreCancion << std::endl;
		delete[] nombreCancion;
	}

	indiceSecundarioAutor.cerrar();
	documentos.cerrar_archivo();
	return RES_OK;
}

int Indexador::_init(std::string & directorioEntrada, std::string & directorioSalida)
{
	int res = RES_OK;
	res += indicePrimario.crear_archivo(directorioSalida+std::string(FILENAME_IDX_PRIM));
	res += indicePrimario.abrir_archivo(directorioSalida+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.crear_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
	res += archivoMaestro.abrir_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
	res += documentos.crear_archivo(directorioSalida+std::string(FILENAME_ID_DOCS));
	res += documentos.abrir_archivo(directorioSalida+std::string(FILENAME_ID_DOCS));
	res += indiceSecundarioAutor.crear(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR));
	res += indiceSecundarioAutor.abrir(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	res += indiceSecundarioTitulo.crear_archivo(directorioSalida+std::string(FILENAME_IDX_SECUN_TITULO));
	res += indiceSecundarioTitulo.abrir_archivo(directorioSalida+std::string(FILENAME_IDX_SECUN_TITULO));
	res += parser.crear(directorioEntrada);
	if (res != RES_OK)
		return RES_ERROR;

	return res;
}

int Indexador::_finalizar()
{
	int res = indicePrimario.cerrar_archivo();
	res += documentos.cerrar_archivo();
	res += indiceSecundarioAutor.cerrar();
	res += indiceSecundarioTitulo.cerrar_archivo();
	return res;
}

int Indexador::indexar (std::string & directorioEntrada, std::string & directorioSalida)
{
	_init(directorioEntrada,directorioSalida);

	ClaveNumerica identificadorCancion(0);

	RegistroCancion regCancion;
	std::string nombreArchivo;

	// Para cada cancion que tengamos...
	while (! parser.fin_directorio())
	{
		int res = parser.obtener_proxima_cancion(regCancion,nombreArchivo);
		if (res != RES_OK)
		{
			std::cout << "No se indexó " << nombreArchivo << " porque no cumple el estandar especificado." << std::endl;
			continue;
		}

		/* ------ guardamos el registro de la cancion en un archivo maestro ------ */

		regCancion.comprimir(compresor);
		long offsetInicialRegCancion = archivoMaestro.agregar_registro(&regCancion);
		
		/* ------ guardamos el numero de documento (clave) y el documento -------- */
		RegistroClave regDoc;
		ClaveX IDdoc;
		IDdoc.set_clave(identificadorCancion.get_dato());

		regDoc.set_clave(IDdoc);
		regDoc.agregar_campo(nombreArchivo.c_str(),nombreArchivo.size());
		this->documentos.agregar(regDoc);

		/* ----- agregamos al indice primario: ID cancion + offset de la cancion ----*/

		RegistroClave regClave;
		ClaveX clave;
		clave.set_clave(identificadorCancion.get_dato());
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
			idCancion << identificadorCancion.get_dato();
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

		int idCancion = identificadorCancion.get_dato();
		regClave3.agregar_campo((char*)&idCancion,sizeof(idCancion));

		indiceSecundarioTitulo.agregar(regClave3);
		

		/** incrementamos el ID de la cancion **/
		identificadorCancion.incrementar();
		

		/* TODO crear el indice invertido para las frases */

		std::cout << "Se indexó " << nombreArchivo << " correctamente!" << std::endl;

	}
	
	_finalizar();

	return RES_OK;
}

