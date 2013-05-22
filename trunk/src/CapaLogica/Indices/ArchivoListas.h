#ifndef ARCHIVOLISTAS_H_INCLUDED
#define ARCHIVOLISTAS_H_INCLUDED

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"

class ArchivoListas
{
    private:
        std::string fileName;
        std::string directorioSalida;
        ManejadorRegistrosVariables archivo;

    public:
        ArchivoListas();
        virtual ~ArchivoListas();
        virtual int crear(std::string directorioSalida, std::string fileNamee);
        virtual int abrir(std::string directorioSalidaa, std::string fileNamee);
        virtual int devolver(RegistroVariable *listaDeCanciones, unsigned short pos_lista);
        virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID);
        virtual int recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID, int *pos);
        virtual int eliminar(std::string directorioSalida, std::string fileNamee);
        virtual unsigned short get_cantidad_registros_ocupados();
};



#endif // ARCHIVOLISTAS_H_INCLUDED
