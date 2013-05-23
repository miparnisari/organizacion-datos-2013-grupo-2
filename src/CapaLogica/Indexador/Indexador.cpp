#include "Indexador.h"


Indexador::Indexador()
{
	
}


Indexador::~Indexador()
{
	
}

int Indexador::init(std::string directorioEntrada, std::string directorioSalida)
{
	int res= indicePrimario.crear(directorioSalida+std::string(FILENAME_IDX_PRIM));
	res += archivoMaestro.crear_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
	res += archivoMaestro.abrir_archivo(directorioSalida+std::string(FILENAME_ARCH_MAESTRO));
	res += indiceSecundarioAutor.crear(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR));
	res += indiceSecundarioAutor.abrir(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR),"rb+");
	res += indiceSecundarioTitulo.crear(directorioSalida+std::string(FILENAME_IDX_SECUN_TITULO));
	res += parser.crear(directorioEntrada);
	if (res != RES_OK)
		return RES_ERROR;

	return res;
}

int Indexador::indexar (std::string directorioEntrada, std::string directorioSalida)
{
	init(directorioEntrada,directorioSalida);

	int contador = 0;
	ClaveX identificadorCancion; // inicialmente vale 0
	identificadorCancion.set_clave(contador);
	
	RegistroCancion regCancion;
	std::string nombreArchivo;

	// Para cada cancion que tengamos...
	while (parser.getNextCancion(regCancion,nombreArchivo) == RES_OK)
	{
		regCancion.comprimir(compresor);
		long offsetInicialRegCancion = archivoMaestro.agregar_registro(&regCancion);
		
		/* ----- agregamos al indice primario: ID cancion + offset de la cancion ----*/

		RegistroClave regClave;
		regClave.set_clave(identificadorCancion);
		regClave.agregar_campo((char*)&offsetInicialRegCancion,sizeof(offsetInicialRegCancion));
		indicePrimario.agregar(regClave);


		/* ----- agregamos al indice por autor: el autor + ID cancion ----*/

		for (unsigned int i = 0; i < regCancion.get_cantidad_autores(); i++)
		{
			RegistroClave regClave2;

			std::string clave = regCancion.get_autor(i);
			clave.push_back((char)contador);

			ClaveX claveAutor;
			claveAutor.set_clave(clave);
			regClave2.set_clave(claveAutor);
//			regClave2.agregar_campo((char*)&contador,sizeof(contador));
			indiceSecundarioAutor.agregar(regClave2);
		}

		/* ----- agregamos al indice por titulo: el titulo + ID cancion ----*/

		RegistroClave regClave3;
		ClaveX claveTitulo;
		claveTitulo.set_clave(regCancion.get_titulo());
		regClave3.set_clave(claveTitulo);
		regClave3.agregar_campo((char*)&contador,sizeof(contador));
		indiceSecundarioTitulo.agregar(regClave3);
		

		/** incrementamos el ID de la cancion **/
		contador++;
		identificadorCancion.set_clave(contador);
		
	}
	
	return RES_OK;
}

