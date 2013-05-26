#include "Indexador.h"


Indexador::Indexador()
{
}


Indexador::~Indexador()
{
	
}

int Indexador::consultar_titulo(std::string & directorioSalida, std::string & titulo)
{
	int res = indiceSecundarioTitulo.abrir_archivo(directorioSalida+std::string(FILENAME_IDX_SECUN_TITULO));
	documentos.abrir_archivo(directorioSalida+std::string(FILENAME_ID_DOCS));
	ClaveX claveTitulo;
	claveTitulo.set_clave(titulo);

	RegistroClave regDevuelto;
	res = indiceSecundarioTitulo.devolver(claveTitulo,&regDevuelto);
	if (res == RES_RECORD_DOESNT_EXIST)
	{
		std::cout << "La cancion " << titulo << " no fue encontrada." << std::endl;
	}
	else {

		// Recupero el IDdoc
		int idDoc;
		regDevuelto.recuperar_campo((char*)&idDoc,1);

		ClaveX claveIdDoc;
		claveIdDoc.set_clave(idDoc);

		// Busco el IDdoc en el archivo de documentos
		RegistroClave regDevuelto2;
		documentos.devolver(claveIdDoc,&regDevuelto2);

		char* nombreCancion = new char[regDevuelto2.get_tamanio_campo(1) +1]();
		nombreCancion[regDevuelto2.get_tamanio_campo(1)] = '\0';
		regDevuelto2.recuperar_campo(nombreCancion,1);

		std::cout << "Nombre de archivo = " << nombreCancion << std::endl;
		delete[] nombreCancion;

	}

	indiceSecundarioTitulo.cerrar_archivo();
	documentos.cerrar_archivo();
	return RES_OK;
}

int Indexador::consultar_autor(std::string & directorioSalida, std::string & unAutor)
{
	indiceSecundarioAutor.abrir(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	documentos.abrir_archivo(directorioSalida+std::string(FILENAME_ID_DOCS));
	IterArbolBMas buscador(indiceSecundarioAutor);
	ClaveX claveInicio;
	claveInicio.set_clave(unAutor + (char)0);
	int res = buscador.start(">=",claveInicio);

	ClaveX claveFin;
	std::string autorSiguiente = unAutor;
	autorSiguiente[autorSiguiente.size()-1] = (char)(unAutor[unAutor.size()-1] + 1);
	claveFin.set_clave(autorSiguiente);

	RegistroClave otraCancion;
	std::cout << "Buscando canciones de " << unAutor << "..." << std::endl;
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
	bool dirEntradaExiste = utilitarios::directorio_existe(directorioEntrada);
	if (! dirEntradaExiste)
	{
		std::cout << "ERROR: El directorio a indexar no existe." << std::endl;
		return RES_DIRECTORY_DOESNT_EXIST;
	}

	bool dirSalidaExiste = utilitarios::directorio_existe(directorioSalida);
	if (! dirSalidaExiste)
	{
		std::cout << "AVISO: El directorio de salida no existe. Se creará." << std::endl;
		mkdir(directorioSalida.c_str(),S_IRWXG);
	}
	else {
		std::cout << "AVISO: Ya existe un indice en " << directorioSalida << ". ¿Que desea hacer?" << std::endl;
		std::cout << "(a) Borrar el indice e indexar desde cero." << std::endl;
		std::cout << "(b) Anexar las canciones de " << directorioEntrada << " al índice." << std::endl;
		char c;
		c = getchar();
	}


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
	return res;
}

int Indexador::_finalizar()
{
	int res = indicePrimario.cerrar_archivo();
	res += documentos.cerrar_archivo();
	res += indiceSecundarioAutor.cerrar();
	int res1 =indiceSecundarioTitulo.cerrar_archivo();
	if (res1 == RES_ERROR)
		cout << "ERROR: el archivo de titulos no se pudo cerrar" << endl;
	return res;
}

int Indexador::indexar (std::string & directorioEntrada, std::string & directorioSalida)
{
	int res = _init(directorioEntrada,directorioSalida);
	if (res != RES_OK) {
		cout << "No se pudieron crear y/o abrir los archivos necesarios." << endl;
		return _finalizar();
	}
	else
	{

		ClaveNumerica identificadorCancion(0);

		RegistroCancion regCancion;
		std::string nombreArchivo;

		// Para cada cancion que tengamos...
		while (! parser.fin_directorio())
		{
			res = parser.obtener_proxima_cancion(regCancion,nombreArchivo);
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
		
		res = _finalizar();

		return RES_OK;
	}
}

