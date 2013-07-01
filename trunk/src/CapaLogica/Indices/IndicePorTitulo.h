#ifndef INDICE_TITULO_H_
#define INDICE_TITULO_H_

#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../Constantes.h"
#include "../../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "Definiciones.h"

class IndicePorTitulo
{
    private:
        std::string nombre_archivo;
        HashingExtensible indice;

    public:
        IndicePorTitulo();

        virtual ~IndicePorTitulo();

        int crear_indice(std::string directorioSalida);
        int eliminar_indice();

        int abrir_indice(std::string directorioSalida);
        int cerrar_indice();

        int agregar(const std::string & titulo, const IDdocumento_t IDcancion);
        //Agrega el IDcancion a la lista de canciones que tienen el titulo

        int buscar(const std::string & titulo, vector<IDdocumento_t> & id_docs);
        //Guarda en id_docs la lista de IDs de canciones que tienen el titulo buscado

        int eliminar(const std::string & titulo, const IDdocumento_t IDcancion);
         //Guarda en id_docs la lista de IDs de canciones que tienen el titulo buscado

};

#endif // INDICE_TITULO_H_
