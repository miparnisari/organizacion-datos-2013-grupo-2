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

	int res = fread(&(this->header),sizeof(header),1,file_handler);
	if (res != 1)
		return RES_ERROR;

	return RES_OK;
}/* Abre un archivo con un modo de apertura especifico.
Para info sobre los modos posibles: http://www.cplusplus.com/reference/cstdio/fopen/
*/


int ManejadorBloques::cerrar_archivo()
{
	if (file_handler == NULL)
		return RES_OK;
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
	bloqueNuevo.marcar_libre();
	bloqueNuevo.actualizar_ref_prox_bloque(header.proximoBloqueLibre);

	int res = escribir_bloque(nombreArchivo,&bloqueNuevo);
	if (res == RES_OK)
	{
		__get_header();
		header.cantidadBloques += 1;
		header.proximoBloqueLibre = header.cantidadBloques;
		__set_header();
	}

	return res;
}/* Agrega un bloque nuevo al archivo. */

int ManejadorBloques::__liberar_bloque(std::string nombreArchivo, unsigned int numBloque)
{
	if (numBloque < 0 || numBloque > this->header.cantidadBloques)
		return RES_ERROR;

	Bloque bloque;
	obtener_bloque(nombreArchivo,&bloque,numBloque);

	bloque.marcar_libre();
	bloque.actualizar_ref_prox_bloque(header.proximoBloqueLibre);
	int res = sobreescribir_bloque(nombreArchivo,&bloque,numBloque);
	if (res == RES_OK)
	{
		__get_header();
		header.cantidadBloques += 1;
		header.proximoBloqueLibre = numBloque;
		__set_header();
		return RES_OK;
	}

	return res;

}/* Marca un bloque como liberado, y lo agrega a la lista de bloques libres. */

unsigned int ManejadorBloques::__get_primer_bloque_libre(std::string nombreArchivo)
{
	__get_header();
	if (header.proximoBloqueLibre == -1)
		__agregar_bloque(nombreArchivo); // Aca se modifica el proximo libre

	return header.proximoBloqueLibre;
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
		Bloque bloque;
		obtener_bloque(nombreArchivo,&bloque,puntero);
		puntero = bloque.obtener_ref_prox_bloque();
	}
	return false;
}/* Verifica si un bloque esta en la lista como libre. */

//int ManejadorBloques::__marcar_libre_bloque(std::string nombreArchivo, unsigned int numBloque)
//{
//	if (numBloque < 0 || numBloque > this->header.cantidadBloques)
//		return RES_ERROR;
//
//	// La lista de bloques libres se maneja como una pila
//	int primerBloque = this->header.proximoBloqueLibre;
//
//	Bloque bloqueAmodificar;
//	obtener_bloque(nombreArchivo, &bloqueAmodificar,numBloque);
//	if (bloqueAmodificar.esta_libre())
//		return RES_OK;
//
//	bloqueAmodificar.actualizar_ref_prox_bloque(primerBloque);
//
//	sobreescribir_bloque(nombreArchivo, &bloqueAmodificar,numBloque);
//
//	this->header.proximoBloqueLibre = numBloque;
//
//	__set_header();
//	return RES_OK;
//}

int ManejadorBloques::__usar_bloque(std::string nombreArchivo,unsigned int numBloque)
{
	// La lista de bloques libres se maneja como una pila
	unsigned int primerBloque = this->header.proximoBloqueLibre;

	// No se puede marcar como usado un bloque que no sea el tope de la pila
	if (numBloque != primerBloque)
		return RES_ERROR;

	Bloque bloqueAmodificar;
	obtener_bloque(nombreArchivo,&bloqueAmodificar,numBloque);

//	if (! bloqueAmodificar.esta_libre())
//		return RES_OK;

	int referencia = bloqueAmodificar.obtener_ref_prox_bloque();
	this->header.proximoBloqueLibre = referencia;
	__set_header();

	return RES_OK;
}

//int ManejadorBloques::agregar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, RegistroVariable* reg)
//{
//	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
//	int res = obtener_bloque(nombreArchivo,&bloque,numBloque);
//	if (res != RES_OK)
//		return RES_ERROR;
//
//	res = bloque.agregar_registro(reg);
//	if (res == RES_OK)
//	{
//		res = sobreescribir_bloque(nombreArchivo,&bloque,numBloque);
//		return res;
//	}
//	return res;
//}

//int ManejadorBloques::eliminar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, unsigned int numReg)
//{
//	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
//	obtener_bloque(nombreArchivo,&bloque,numBloque);
//
//	int res = bloque.remover_registro(numReg);
//	if (res == RES_OK)
//	{
//		return sobreescribir_bloque(nombreArchivo,&bloque,numBloque);
//	}
//	return res;
//}

//int ManejadorBloques::get_registro_de_bloque(std::string nombreArchivo, unsigned int numReg, unsigned int numBloque, RegistroVariable* reg)
//{
//	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
//
//	int res = obtener_bloque(nombreArchivo,&bloque,numBloque);
//	if (res != RES_OK)
//	{
//		return res;
//	}
//
//	return bloque.recuperar_registro(reg,numReg);
//}

int ManejadorBloques::escribir_bloque(std::string nombreArchivo, Bloque* bloque)
{
	if (bloque->esta_vacio() || bloque == NULL)
		return RES_OK;

	unsigned int numeroBloque = __get_primer_bloque_libre(nombreArchivo);
	return sobreescribir_bloque(nombreArchivo,bloque,numeroBloque);

}

int ManejadorBloques:: sobreescribir_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	if (file_handler == NULL || bloque == NULL || numBloque < 0 || numBloque > header.cantidadBloques)
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



int ManejadorBloques :: obtener_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	if (file_handler == NULL || bloque == NULL || numBloque < 0 || numBloque > header.cantidadBloques)
		return RES_ERROR;

	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	if (__esta_en_lista_de_libres(nombreArchivo, numBloque))
		return RES_ERROR;

	// Salteamos el header
	long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res;
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0)
		return RES_ERROR;

	// Leemos el bloque
	char* buffer = new char[header.tamanioBloque];
	res = fread(buffer,this->header.tamanioBloque,1,this->file_handler);
	bloque->desempaquetar(buffer);
	delete[] buffer;
	if (res != 1)
	{
		return RES_ERROR;
	}
	return RES_OK;
}/* Devuelve un bloque del archivo.
PRECONDICION: el archivo se debe abrir en modo lectura.
POSTCONDICION: se lo debe cerrar. */

int ManejadorBloques::get_proximo_bloque_libre()
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
