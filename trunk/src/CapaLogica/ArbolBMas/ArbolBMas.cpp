#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{

}
ArbolBMas::~ArbolBMas()
{}

int ArbolBMas::crear (std::string dir, std::string fileName)
{
	unsigned int tamanioBloque = BLOQUE_TAM_DEFAULT;
	unsigned int minCantidadBytes = 0;
	unsigned int maxCantidadBytes = 0;
	Bloque* bloque = new Bloque(tamanioBloque);
	NodoInterno raiz (minCantidadBytes, maxCantidadBytes);
	raiz.empaquetar(bloque);
	return RES_OK;
}

int ArbolBMas::eliminar (std::string dir, std::string fileName)
{
	return RES_OK;
}

int ArbolBMas::abrir (std::string dir, std::string fileName, std::string mode)
{
	return RES_OK;
}

int ArbolBMas::cerrar ()
{
	return RES_OK;
}

int ArbolBMas::agregar(RegistroVariable & reg)
{
	return RES_OK;
}
int ArbolBMas::eliminar(RegistroVariable & reg)
{
	return RES_OK;
}

int ArbolBMas::buscar(RegistroVariable & reg)
{
	return RES_OK;
}