#include "ManejadorBloques.h"

ManejadorBloques::ManejadorBloques(unsigned int tamBloque)
{
	header.tamanioBloque = tamBloque;
	header.cantidadBloques = 0;
	header.proximoBloqueLibre = 0;
	file_handler = NULL;
}

ManejadorBloques::~ManejadorBloques()
{
	fclose(file_handler);
}

int ManejadorBloques::abrir_archivo(std::string nombreArchivo, std::string modo)
{
	file_handler = fopen(nombreArchivo.c_str(), modo.c_str());
	if (file_handler == NULL)
		return RES_ERROR;

	int res = fread(&(this->header),sizeof(header),1,file_handler);
	if (res != sizeof(header))
		return RES_ERROR;

	return RES_OK;

}

int ManejadorBloques::__get_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fread(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != sizeof(this->header))
		return RES_ERROR;
	return RES_OK;
}

int ManejadorBloques::__set_header()
{
	fseek(this->file_handler,0,SEEK_SET);
	int res = fwrite(&(this->header),sizeof(this->header),1,this->file_handler);
	if (res != sizeof(this->header))
		return RES_ERROR;
	return RES_OK;
}

int ManejadorBloques::__marcar_libre_bloque(std::string nombreArchivo,unsigned int numBloque)
{

	if (numBloque < 0 || numBloque > this->header.cantidadBloques)
		return RES_ERROR;

	// La lista de bloques libres se maneja como una pila
	int primerBloque = this->header.proximoBloqueLibre;

	Bloque bloqueAmodificar;
	obtener_bloque(nombreArchivo, &bloqueAmodificar,numBloque);
	if (bloqueAmodificar.esta_libre())
		return RES_OK;

	bloqueAmodificar.actualizar_ref_prox_bloque(primerBloque);

	actualizar_bloque(nombreArchivo, &bloqueAmodificar,numBloque);

	this->header.proximoBloqueLibre = numBloque;

	__set_header();
	return RES_OK;
}

int ManejadorBloques::__marcar_usado_bloque(std::string nombreArchivo,unsigned int numBloque)
{
	if (numBloque < 0 || numBloque > this->header.cantidadBloques)
		return RES_ERROR;

	// La lista de bloques libres se maneja como una pila
	unsigned int primerBloque = this->header.proximoBloqueLibre;

	// No se puede marcar como usado un bloque que no sea el tope de la pila
	if (numBloque != primerBloque)
		return RES_ERROR;

	Bloque bloqueAmodificar;
	obtener_bloque(nombreArchivo,&bloqueAmodificar,numBloque);

	if (! bloqueAmodificar.esta_libre())
		return RES_OK;

	int referencia = bloqueAmodificar.obtener_ref_prox_bloque();
	this->header.proximoBloqueLibre = referencia;
	__set_header();

	return RES_OK;

}

int ManejadorBloques::crear_archivo(std::string nombreArchivo)
{
	file_handler = fopen(nombreArchivo.c_str(), "wb+");

	__set_header();

	fclose(file_handler);

	return RES_OK;
}

int ManejadorBloques:: actualizar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	if (bloque->esta_vacio())
		bloque->marcar_libre_bloque(numBloque);

	int res;
	res = abrir_archivo(nombreArchivo,"wb+");
	if (res != RES_OK)
		return RES_ERROR;

	// Salteamos el header
	unsigned long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0) {
		fclose(file_handler);
		return RES_ERROR;
	}

	// Escribimos el bloque
	res = fwrite(bloque,this->header.tamanioBloque,1,this->file_handler);
	offset = ftell(this->file_handler);

	// Caso de que sea un append de bloque
	if (offset > sizeof(header) + header.cantidadBloques*header.tamanioBloque)
	{
		this->header.cantidadBloques += 1;
		__set_header();

	}
	if (res != 1) {
		fclose(file_handler);
		return RES_ERROR;
	}

	fclose(file_handler);
	return RES_OK;
}

int ManejadorBloques :: obtener_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	abrir_archivo(nombreArchivo,"rb");

	// Salteamos el header
	long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res;
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0) {
		fclose(file_handler);
		return RES_ERROR;
	}

	// Leemos el bloque
	res = fread(bloque,this->header.tamanioBloque,1,this->file_handler);
	if (res != 1)
	{
		fclose(file_handler);
		return RES_ERROR;
	}
	fclose(file_handler);
	return RES_OK;
}

int ManejadorBloques::get_proximo_bloque_libre()
{
	return this->header.proximoBloqueLibre;
}/* precondicion: antes abrir el archivo! */

int ManejadorBloques::get_tamanio_bloque()
{
	return this->header.tamanioBloque;
} /* precondicion: antes abrir el archivo! */

int ManejadorBloques::get_cantidad_bloques()
{
	return this->header.cantidadBloques;
}/* precondicion: antes abrir el archivo! */

