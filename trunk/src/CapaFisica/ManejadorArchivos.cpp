#include "ManejadorArchivos.h"

ManejadorArchivos::ManejadorArchivos()
{
}

int ManejadorArchivos::crear_archivo(std::string p_nombreArchivo)
{
	if (! utilitarios::validFileName(p_nombreArchivo) )
		return RES_ERROR;

	std::ofstream archivoNuevo(p_nombreArchivo.c_str());
	if (!archivoNuevo.is_open())
		return RES_ERROR;

	archivoNuevo.close();

	return RES_OK;

}


bool ManejadorArchivos::archivo_existe(std::string nombreArchivo)const throw()
{
	if (! utilitarios::validFileName(nombreArchivo) )
		return RES_ERROR;

	std::ifstream archivo;
	archivo.open(nombreArchivo.c_str(),std::ios::in);
	bool resultado= archivo.good();

	if(resultado){
		archivo.seekg(0,std::ios::end);
		archivo.close();
	}

	return resultado;
}


int ManejadorArchivos::eliminar_archivo(std::string nombreArchivo)const throw()
{
	int res;

	if (! utilitarios::validFileName(nombreArchivo) )
		return RES_ERROR;

	if(!archivo_existe(nombreArchivo))
		return RES_FILE_DOESNT_EXIST;

	res = remove(nombreArchivo.c_str());
	if (res == 0)
		return RES_OK;

	return RES_ERROR;
}
