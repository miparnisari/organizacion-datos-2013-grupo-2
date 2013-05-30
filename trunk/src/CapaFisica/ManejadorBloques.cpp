#include "ManejadorBloques.h"

ManejadorBloques::ManejadorBloques()
{
	header.tamanioBloque = 0;
	header.cantidadBloques = 0;
	header.proximoBloqueLibre = -1;
	file_handler = NULL;
	nombreArchivo = "";
}

ManejadorBloques::~ManejadorBloques()
{
}

void ManejadorBloques::set_ruta(std::string ruta)
{
	nombreArchivo = ruta;
}

std::string ManejadorBloques::get_ruta()
{
	return nombreArchivo;
}

unsigned int ManejadorBloques::get_tamanio_bloque()
{
	return this->header.tamanioBloque;
} /* precondicion: antes abrir el archivo! */

unsigned int ManejadorBloques::get_cantidad_bloques()
{
	return this->header.cantidadBloques;
}/* precondicion: antes abrir el archivo! */

int ManejadorBloques::abrir_archivo(std::string p_nombreArchivo, std::string modo)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;
	nombreArchivo = p_nombreArchivo;
	file_handler = fopen(nombreArchivo.c_str(), modo.c_str());
	if (file_handler == NULL)
		return RES_ERROR;

	return __get_header();
}

int ManejadorBloques::cerrar_archivo()
{
	if (file_handler == NULL)
		return RES_OK;
	__set_header();
	int res = fclose(file_handler);
	file_handler = NULL;
	if (res == 0)
		return RES_OK;
	return RES_ERROR;
}

int ManejadorBloques::crear_archivo(std::string p_nombreArchivo, unsigned int tamBloque)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;
	header.cantidadBloques = 0;
	header.proximoBloqueLibre = -1;
	nombreArchivo = p_nombreArchivo;
	file_handler = fopen(nombreArchivo.c_str(), "wb+");
	if (file_handler == NULL)
		return RES_ERROR;

	header.tamanioBloque = tamBloque;

	int res = __set_header();
	if (res == RES_ERROR)
		return RES_ERROR;

	res = cerrar_archivo();
	return res;
}

int ManejadorBloques::eliminar_archivo(std::string p_nombreArchivo)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;
	int res = remove (p_nombreArchivo.c_str() );
	if (res == 0)
		return RES_OK;
	return RES_FILE_DOESNT_EXIST;
} /* Borra un archivo del disco. */

int ManejadorBloques::__escribir_bloque(const Bloque* bloque, unsigned int offset)
{
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
}/* Escribe un bloque dado su offset.
PRECONDICION: el offset tiene que ser del comienzo del bloque. El archivo debe estar abierto. */

int ManejadorBloques::__agregar_bloque_al_final()
{
	Bloque bloqueNuevo(this->header.tamanioBloque);
	bloqueNuevo.actualizar_ref_prox_bloque(header.proximoBloqueLibre);

	unsigned long int offset = sizeof(header) + (header.tamanioBloque *header.cantidadBloques);
	int res = __escribir_bloque(&bloqueNuevo,offset);
	if (res == RES_ERROR)
		return res;

	header.proximoBloqueLibre = header.cantidadBloques;
	header.cantidadBloques += 1;

	return RES_OK;
}/* Agrega un bloque nuevo al archivo.
PRECONDICION: el archivo debe estar abierto.  */

int ManejadorBloques::__liberar_bloque(Bloque* bloque, unsigned int numBloque)
{
	unsigned int copiaProximoLibre = header.proximoBloqueLibre;
	header.proximoBloqueLibre = numBloque;
	return bloque->actualizar_ref_prox_bloque(copiaProximoLibre);

}/* Marca un bloque como liberado, y lo agrega a la lista de bloques libres.
PRECONDICION: el archivo debe estar abierto. */

unsigned int ManejadorBloques::__get_primer_bloque_libre()
{
	if (header.proximoBloqueLibre == -1)
		__agregar_bloque_al_final(); //Aca se modifica header.proximoBloqueLibre

	return header.proximoBloqueLibre;
}/* PRECONDICION: el archivo debe estar abierto. */

int ManejadorBloques::__get_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fread(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != 1)
		return RES_ERROR;
	return RES_OK;
}/*PRECONDICION: el archivo debe estar abierto. */

int ManejadorBloques::__set_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fwrite(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != 1)
		return RES_ERROR;
	return RES_OK;
}/*PRECONDICION: el archivo debe estar abierto. */

bool ManejadorBloques::__num_bloque_es_valido(unsigned int numBloque)
{
	if (numBloque > header.cantidadBloques)
		return false;
	if (header.cantidadBloques == 0)
		return false;
	if (header.cantidadBloques >= 1 && numBloque >= header.cantidadBloques)
		return false;
	return true;
}

bool ManejadorBloques::__es_tope_de_pila(int numBloque)
{
	return (header.proximoBloqueLibre == numBloque);
}

int ManejadorBloques::__usar_bloque(Bloque* bloque, unsigned int numBloque)
{
	int referencia = bloque->obtener_ref_prox_bloque();
	this->header.proximoBloqueLibre = referencia;
	return RES_OK;
}

int ManejadorBloques::escribir_bloque(Bloque* bloque)
{
	if (file_handler == NULL || bloque == NULL)
		return RES_ERROR;

	unsigned int numeroBloque = __get_primer_bloque_libre();
	int res = sobreescribir_bloque(bloque,numeroBloque);
	if (res != RES_ERROR)
		return numeroBloque;

	return res;
}

int ManejadorBloques:: sobreescribir_bloque(Bloque* bloque, unsigned int numBloque)
{
	if (file_handler == NULL || bloque == NULL || !__num_bloque_es_valido(numBloque))
		return RES_ERROR;

	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	/* Fin comprobaciones de rutina */

	bool nuevoEstaVacio = bloque->esta_vacio();
	Bloque* anterior = obtener_bloque(numBloque); // Bloque que voy a pisar

	// No se puede sobreescribir un bloque antes eliminado si el mismo no era el tope de la pile de libres
	if (anterior->fue_eliminado() && !__es_tope_de_pila(numBloque)){
		delete(anterior);
		return RES_ERROR;
	}

	// Caso especial en que el bloque estaba libre y lo voy a marcar como usado
	if (__es_tope_de_pila(numBloque))
		__usar_bloque(anterior, numBloque);

	// Caso especial en que el bloque estaba ocupado y lo voy a marcar como libre
	if (nuevoEstaVacio && ! anterior->esta_vacio())
		__liberar_bloque(bloque, numBloque);

	delete(anterior);

	unsigned long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	return __escribir_bloque(bloque,offset);


}

Bloque* ManejadorBloques::crear_bloque()
{
	if (this->file_handler == NULL)
		return NULL;
	return new Bloque(header.tamanioBloque);
}

Bloque* ManejadorBloques :: obtener_bloque(unsigned int numBloque)
{
	if (file_handler == NULL || !__num_bloque_es_valido(numBloque))
		return NULL;

	// Salteamos el header
	long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res;
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0)
		return NULL;

	// Leemos el bloque
	Bloque* bloqueLeido = new Bloque(header.tamanioBloque);
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
}
