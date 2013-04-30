#include "ManejadorBloques.h"

ManejadorBloques::ManejadorBloques(unsigned int tamBloque, unsigned int min, unsigned int max)
{
	header.tamanioBloque = tamBloque;
	header.cantidadBloques = 0;
	header.proximoBloqueLibre = 0;
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
}

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
		return RES_ERROR;

	__set_header();

	Bloque bloque0(this->header.tamanioBloque,header.minRegsBloque, header.maxRegsBloque);
	int res = actualizar_bloque(nombreArchivo,&bloque0,0);
	cerrar_archivo();

	return res;
}

int ManejadorBloques::eliminar_archivo(std::string nombreArchivo)
{
	int res = remove (nombreArchivo.c_str() );
	if (res == 0)
		return RES_OK;
	return RES_FILE_DOESNT_EXIST;
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

int ManejadorBloques::__marcar_libre_bloque(std::string nombreArchivo, unsigned int numBloque)
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

int ManejadorBloques::agregar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, RegistroVariable* reg)
{
	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
	int res = obtener_bloque(nombreArchivo,&bloque,numBloque);
	if (res != RES_OK)
	{
		std::cout << "falló obtener bloque" << std::endl;
		return RES_ERROR;
	}

	res = bloque.agregar_registro(reg);
	if (res == RES_OK)
	{
		res = actualizar_bloque(nombreArchivo,&bloque,numBloque);
		if (res != RES_OK)
		{
			std::cout << "falló actualizar bloque" << std::endl;
		}
		return res;
	}
	std:: cout << "falló agregar registro en bloque"  << std::endl;
	return res;
}

int ManejadorBloques::eliminar_registro_en_bloque(std::string nombreArchivo, unsigned int numBloque, unsigned int numReg)
{
	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);
	obtener_bloque(nombreArchivo,&bloque,numBloque);

	int res = bloque.remover_registro(numReg);
	if (res == RES_OK)
	{
		return actualizar_bloque(nombreArchivo,&bloque,numBloque);
	}
	return res;
}

int ManejadorBloques::get_registro_de_bloque(std::string nombreArchivo, unsigned int numReg, unsigned int numBloque, RegistroVariable* reg)
{
	Bloque bloque(header.tamanioBloque,header.minRegsBloque,header.maxRegsBloque);

	int res = obtener_bloque(nombreArchivo,&bloque,numBloque);
	if (res != RES_OK)
	{
		return res;
	}

	return bloque.recuperar_registro(reg,numReg);
}

int ManejadorBloques:: actualizar_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	if (file_handler == NULL)
		return RES_ERROR;
	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
		return RES_ERROR;

	if (bloque->esta_vacio())
		bloque->marcar_libre_bloque(numBloque);

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
	offset = ftell(this->file_handler);

	// Caso de que sea un append de bloque
	if (offset > sizeof(header) + header.cantidadBloques*header.tamanioBloque)
	{
		this->header.cantidadBloques += 1;
		__set_header();

	}
	if (res != 1) {
		return RES_ERROR;
	}

	return RES_OK;
}/* Devuelve un bloque del archivo. PRECONDICION: el archivo se debe abrir en modo escritura.
POSTCONDICION: se lo debe cerrar. */

int ManejadorBloques :: obtener_bloque(std::string nombreArchivo, Bloque* bloque, unsigned int numBloque)
{
	// El tamanio del bloque debe coincidir
	if (bloque->get_tamanio_bloque() != this->header.tamanioBloque)
	{
		std::cout << "tamanio bloque no coincide" << std::endl;
		return RES_ERROR;
	}

	// Salteamos el header
	long int offset = sizeof(this->header) + (this->header.tamanioBloque * numBloque);

	// Seek al bloque
	int res;
	res = fseek(this->file_handler,offset,SEEK_SET);
	if (res != 0)
	{
		std::cout << "no pude hacer seek" << std::endl;
		return RES_ERROR;
	}

	// Leemos el bloque
	char* buffer = new char[header.tamanioBloque];
	res = fread(buffer,this->header.tamanioBloque,1,this->file_handler);
	bloque->empaquetar(buffer);
	delete[] buffer;
	if (res != 1)
	{
		std::cout << "no pude leer el bloque" << std::endl;
		return RES_ERROR;
	}
	return RES_OK;
}/* Devuelve un bloque del archivo. PRECONDICION: el archivo se debe abrir en modo lectura.
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
