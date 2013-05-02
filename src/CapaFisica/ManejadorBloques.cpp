#include "ManejadorBloques.h"

ManejadorBloques::ManejadorBloques(unsigned int tamBloque, unsigned int min, unsigned int max)
{
	header.tamanioBloque = tamBloque;
	header.cantidadBloques = 0;
	header.proximoBloqueLibre = -1;
	header.minRegsBloque = min;
	header.maxRegsBloque = max;
	file_handler = NULL;
}

ManejadorBloques::~ManejadorBloques()
{
}

int ManejadorBloques::abrir_archivo(std::string nombreArchivo, std::string modo)
{
	file_handler = fopen(nombreArchivo.c_str(), modo.c_str());
	if (file_handler == NULL)
		return RES_ERROR;

	return __get_header();

}/* Abre un archivo con un modo de apertura especifico.
Para info sobre los modos posibles: http://www.cplusplus.com/reference/cstdio/fopen/
*/


int ManejadorBloques::cerrar_archivo()
{
	if (file_handler == NULL)
		return RES_OK;
	__set_header();
	int res = fclose(file_handler);
	if (res == 0)
		return RES_OK;
	return RES_ERROR;
}

int ManejadorBloques::crear_archivo(std::string nombreArchivo)
{
	file_handler = fopen(nombreArchivo.c_str(), "wb+");
	if (file_handler == NULL)
	{
		std::cout << "file_handler == NULL" << std::endl;
		return RES_ERROR;
	}

	int res = __set_header();
	if (res == RES_ERROR)
	{
		std::cout << "__set_header() falló" << std::endl;
		return RES_ERROR;
	}
	res = cerrar_archivo();
	return res;
} /* Crea un archivo de bloques nuevo, sin bloques para usar.
Si el archivo ya existia, sus contenidos se pisaran. */

int ManejadorBloques::eliminar_archivo(std::string nombreArchivo)
{
	int res = remove (nombreArchivo.c_str() );
	if (res == 0)
		return RES_OK;
	return RES_FILE_DOESNT_EXIST;
} /* Borra un archivo del disco. */

int ManejadorBloques::__agregar_bloque(std::string nombreArchivo)
{
	Bloque bloqueNuevo(this->header.tamanioBloque,header.minRegsBloque, header.maxRegsBloque);
//	bloqueNuevo.marcar_libre();
	bloqueNuevo.actualizar_ref_prox_bloque(header.proximoBloqueLibre);

	int res = escribir_bloque(nombreArchivo,&bloqueNuevo);
	if (res == RES_OK)
	{
		header.cantidadBloques += 1;
		header.proximoBloqueLibre = header.cantidadBloques - 1;
	}

	return res;
}/* Agrega un bloque nuevo al archivo. */

int ManejadorBloques::__liberar_bloque(std::string nombreArchivo, unsigned int numBloque)
{
	if (numBloque < 0 || numBloque > this->header.cantidadBloques)
		return RES_ERROR;

	Bloque* bloque = obtener_bloque(nombreArchivo,numBloque);
	if (bloque == NULL)
		return RES_ERROR;

//	bloque->marcar_libre();
	bloque->actualizar_ref_prox_bloque(header.proximoBloqueLibre);
	int res = sobreescribir_bloque(nombreArchivo,bloque,numBloque);
	if (res == RES_OK)
	{
		delete(bloque);
		header.proximoBloqueLibre = numBloque;
		return RES_OK;
	}

	return res;

}/* Marca un bloque como liberado, y lo agrega a la lista de bloques libres. */

unsigned int ManejadorBloques::__get_primer_bloque_libre(std::string nombreArchivo)
{
	if (header.proximoBloqueLibre == -1)
	{
		__agregar_bloque(nombreArchivo);
	}

	return header.proximoBloqueLibre;
}

int ManejadorBloques::__get_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fread(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != 1)
		return RES_ERROR;
	return RES_OK;
}

int ManejadorBloques::__set_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fwrite(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != 1)
		return RES_ERROR;
	return RES_OK;
}

bool ManejadorBloques::__esta_en_lista_de_libres(std::string nombreArchivo, unsigned int numBloque)
{
	unsigned int puntero = header.proximoBloqueLibre;
	while (puntero != -1)
	{
		if (puntero == numBloque) return true;
		Bloque* bloque = obtener_bloque(nombreArchivo,puntero);
		puntero = bloque->obtener_ref_prox_bloque();
		delete(bloque);
	}
	return false;
}/* Verifica si un bloque esta en la lista como libre. */

int ManejadorBloques::__usar_bloque(std::string nombreArchivo,unsigned int numBloque)
{
	// La lista de bloques libres se maneja como una pila
	unsigned int primerBloque = this->header.proximoBloqueLibre;

	// No se puede marcar como usado un bloque que no sea el tope de la pila
	if (numBloque != primerBloque)
		return RES_ERROR;

	Bloque* bloqueAmodificar = obtener_bloque(nombreArchivo,numBloque);

//	if (! bloqueAmodificar.esta_libre())
//		return RES_OK;

	int referencia = bloqueAmodificar->obtener_ref_prox_bloque();
	this->header.proximoBloqueLibre = referencia;
	delete(bloqueAmodificar);
	return RES_OK;
}


int ManejadorBloques::escribir_bloque(std::string nombreArchivo, const Bloque* bloque)
{
	if (file_handler == NULL)
		return RES_ERROR;
	if (bloque == NULL)
		return RES_ERROR;
	if (bloque->esta_vacio())
		return RES_OK;

	unsigned int numeroBloque = __get_primer_bloque_libre(nombreArchivo);
	int res = sobreescribir_bloque(nombreArchivo,bloque,numeroBloque);
	if (res != RES_ERROR)
		return numeroBloque;

	return res;
}

int ManejadorBloques:: sobreescribir_bloque(std::string nombreArchivo, const Bloque* bloque, unsigned int numBloque)
{
	if (file_handler == NULL)
		return RES_ERROR;

	if (numBloque >= 1 && header.cantidadBloques == 0)
		return RES_ERROR;

	if (header.cantidadBloques >= 1 && numBloque >= header.cantidadBloques)
		return RES_ERROR;

	if (file_handler == NULL || bloque == NULL || numBloque < 0)
		return RES_ERROR;

	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	// Si el bloque nuevo esta vacio, lo agrego a la pila de bloques libres
	if (bloque->esta_vacio())
		return __liberar_bloque(nombreArchivo,numBloque);

	// El bloque nuevo contiene cosas y el bloque viejo esta en la lista de libres...
	// me fijo si lo puedo marcar como libre.
	if (__esta_en_lista_de_libres(nombreArchivo,numBloque))
	{
		int res = __usar_bloque(nombreArchivo,numBloque);
		if (res == RES_ERROR)
			return res;
	}

	// Salteamos el header
	unsigned long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0) {
		return RES_ERROR;
	}

	// Escribimos el bloque
	char* buffer = new char[header.tamanioBloque];
	bloque->empaquetar(buffer);
	res = fwrite(buffer,this->header.tamanioBloque,1,this->file_handler);
	delete[] buffer;
	if (res != 1) {
		return RES_ERROR;
	}

	return RES_OK;
}/* Devuelve un bloque del archivo.
PRECONDICION: el archivo se debe abrir en modo escritura.
POSTCONDICION: se lo debe cerrar. */

Bloque* ManejadorBloques :: obtener_bloque(std::string nombreArchivo, unsigned int numBloque)
{
	if (file_handler == NULL || numBloque < 0 || numBloque > header.cantidadBloques)
		return NULL;

	if (__esta_en_lista_de_libres(nombreArchivo, numBloque))
		return NULL;

	// Salteamos el header
	long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res;
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0)
		return NULL;

	// Leemos el bloque
	Bloque* bloqueLeido = new Bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
	char* buffer = new char[header.tamanioBloque];
	res = fread(buffer,this->header.tamanioBloque,1,this->file_handler);
	if (res != 1)
	{
		delete[] buffer;
		delete(bloqueLeido);
		return NULL;
	}
	bloqueLeido->desempaquetar(buffer);
	delete[] buffer;
	return bloqueLeido;
}/* Devuelve un bloque del archivo.
PRECONDICION: el archivo se debe abrir en modo lectura.
POSTCONDICION: se lo debe cerrar. */

int ManejadorBloques::get_primer_bloque_libre()
{
	return this->header.proximoBloqueLibre;
}/* precondicion: antes abrir el archivo! */

unsigned int ManejadorBloques::get_tamanio_bloque()
{
	return this->header.tamanioBloque;
} /* precondicion: antes abrir el archivo! */

unsigned int ManejadorBloques::get_cantidad_bloques()
{
	return this->header.cantidadBloques;
}/* precondicion: antes abrir el archivo! */

unsigned int ManejadorBloques::get_max_regs_bloque()
{
	return this->header.maxRegsBloque;
}/* precondicion: antes abrir el archivo! */

unsigned int ManejadorBloques::get_min_regs_bloque()
{
	return this->header.minRegsBloque;
}/* precondicion: antes abrir el archivo! */
