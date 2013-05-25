#include "Indexador.h"


Indexador::Indexador()
{
}


Indexador::~Indexador()
{
	
}

int Indexador::_init(std::string directorioEntrada, std::string directorioSalida)
{
	int res= indicePrimario.crear_archivo(directorioSalida+std::string(FILENAME_IDX_PRIM));
	res += indicePrimario.abrir_archivo(directorioSalida+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.crear_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
	res += archivoMaestro.abrir_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
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
	res += indiceSecundarioAutor.cerrar();
	res += indiceSecundarioTitulo.cerrar_archivo();
	return res;
}

int Indexador::indexar (std::string directorioEntrada, std::string directorioSalida)
{
	_init(directorioEntrada,directorioSalida);

	ClaveNumerica identificadorCancion(0);

	RegistroCancion regCancion;
	std::string nombreArchivo;

	// Para cada cancion que tengamos...
	while (! parser.fin_directorio())
	{
		parser.obtener_proxima_cancion(regCancion,nombreArchivo);
		std::cout << "Nombre archivo = " << nombreArchivo << std::endl;

		/* ------ guardamos el registro de la cancion en un archivo maestro ------ */

		regCancion.comprimir(compresor);
		long offsetInicialRegCancion = archivoMaestro.agregar_registro(&regCancion);
		
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
			std::string clave = regCancion.get_autor(i);
			clave.push_back((char)identificadorCancion.get_dato());
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

	}
	
	_finalizar();

	return RES_OK;
}

