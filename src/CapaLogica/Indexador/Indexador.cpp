#include "Indexador.h"


Indexador::Indexador()
{
	
}


Indexador::~Indexador()
{
	
}

int Indexador::indexar (std::string directorioEntrada, std::string directorioSalida)
{
	// clave = autor + id cancion
	ArbolBMas indiceSecundarioAutor;
	indiceSecundarioAutor.crear(directorioSalida+std::string(FILENAME_IDX_SECUN_AUTOR));
	
	// clave = titulo
//	HashingExtensible indiceSecundarioTitulo;
//	indiceSecundarioTitulo.create(directorioSalida, std::string(FILENAME_IDX_SECUN_TITULO));
	
	// clave = id cancion
//	HashingExtensible indicePrimario;
//	indicePrimario.create(directorioSalida, std::string(FILENAME_IDX_PRIM));
	
	ManejadorRegistrosVariables archivoMaestro;	
	
	ParserCanciones parser;
	parser.crear(directorioEntrada);
	RegistroCancion regCancion;
	
	CompresorPPMC compresor;
	
	ClaveNumerica identificadorCancion ; // inicialmente vale 0
	
	// Para cada cancion que tengamos...
	while (parser.getNextCancion(regCancion) == RES_OK)
	{
		regCancion.comprimir(compresor);
		
//		int offsetInicialRegCancion;
		//archivoMaestro.agregar_registro(directorioSalida+std::string(FILENAME_ARCH_MAESTRO),std::string(regCancion.get_buffer()));
		//FIXME esta funcion fue modificada
		
		// Cargamos en el indice primario el offset al registro
//		int longitudCampo[1] = {sizeof(int)};
		RegistroVariable regOffset;
		//~ FIXME RegistroFijo regOffset(1, longitudCampo); //Un registro tiene un solo campo de 1 int
		//~ FIXME regOffset.pack((char*)offsetInicialRegCancion);
//		indicePrimario.agregar(regOffset);
		
		// Creamos el indice secundario por autor
		// <autor,ID cancion> van a los nodos hoja del arbol B+
		for (int i = 0; i < regCancion.get_cantidad_autores(); i++)
		{
			//RegistroVariable regVar;
			//~ FIXME regVar.pack(regCancion.getAutor(i));
			// FIXME regVar.pack(std::string(identificadorCancion));
			//indiceSecundarioAutor.agregar(regVar);
			//FIXME aca se creaba y se buscaba insertar un RegistroVariable en el arbol,
			//el cual DEBE Recibir un RegistroClave
			RegistroClave registro;
			indiceSecundarioAutor.agregar(registro);
		}
		
		// Creamos el indice secundario por titulo
		RegistroVariable regVar;
		//~ FIXME regVar.pack(regCancion.getTitulo());
		//FIXME regVar.pack(std::string(identificadorCancion));
//		indiceSecundarioTitulo.agregar(regVar);
		
		// Incrementamos la clave
		
		identificadorCancion.incrementar();
		
	}
	
	return RES_OK;
}

