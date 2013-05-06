#ifndef MANEJADORDATOSBASICOS_H_
#define MANEJADORDATOSBASICOS_H_

#include "ManejadorArchivos.h"

struct mdb_header
{
	unsigned int tamanio_archivo;
};

template<typename TipoDato>
class ManejadorArchivoDatosBasicos: public ManejadorArchivos {

	public:
		ManejadorArchivoDatosBasicos();
		virtual ~ManejadorArchivoDatosBasicos();

		int truncar(long longitudDeseada);
		int agregar(const TipoDato dato);
		int escribir(const TipoDato dato, int pos);
		int leer(TipoDato* dato, int pos);

		int crear_archivo(std::string nombreArchivo);
		int eliminar_archivo(std::string nombreArchivo);

		int abrir_archivo(std::string nombreArchivo, std::string modo);
		int cerrar_archivo();

	private:
		struct mdb_header header;
		FILE* file_handler;
		std::string nombreArchivo;
		int __get_header();
		int __set_header();
		int __get_tamanio_archivo();
};

#endif /* MANEJADORDATOSBASICOS_H_ */
