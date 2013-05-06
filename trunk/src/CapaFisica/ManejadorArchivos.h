#ifndef MANEJADORARCHIVOS_H_
#define MANEJADORARCHIVOS_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../Constantes.h"
#include "../Utilitarios/Utilitarios.h"


class ManejadorArchivos {

	public:
		ManejadorArchivos();
		virtual ~ManejadorArchivos() {};

		virtual int crear_archivo(std::string nombreArchivo);
		virtual bool archivo_existe(std::string nombreArchivo)const throw();
		virtual int eliminar_archivo(std::string nombreArchivo)const throw();

};

#endif /* MANEJADORARCHIVOS_H_ */
