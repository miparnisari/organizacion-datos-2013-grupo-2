#ifndef MANEJADORDATOSBASICOS_H_
#define MANEJADORDATOSBASICOS_H_

#include "ManejadorArchivos.h"

template<typename TipoDato>

class ManejadorDatosBasicos: public ManejadorArchivos {

	public:
		ManejadorDatosBasicos();
		virtual ~ManejadorDatosBasicos();

		int agregar(const TipoDato dato);
		int escribir(const TipoDato dato, int pos);
		int leer(TipoDato dato, int pos);

		int crear_archivo(std::string nombreArchivo, unsigned int tamBloque);
		int eliminar_archivo(std::string nombreArchivo);

		int abrir_archivo(std::string nombreArchivo, std::string modo);
		int cerrar_archivo();

	private:
		FILE* file_handler;
		std::string nombreArchivo;

		int get_tamanio_archivo();
};

#endif /* MANEJADORDATOSBASICOS_H_ */
