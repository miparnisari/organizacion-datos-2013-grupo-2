#ifndef INDICEINVERTIDO_H_INCLUDED
#define INDICEINVERTIDO_H_INCLUDED

#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../Constantes.h"

class IndiceInvertido
{
    //Atributos
    protected:
        std::string fileName;
        ManejadorRegistrosVariables archivo_terminos;
        ArbolBMas vocabulario;
        ManejadorBloques listas_invertidas;
        ManejadorRegistrosVariables archivo_coincidencias;

    public:
        virtual int crear_indice(std::string directorioSalida, std::string fileNamee);
        virtual int abrir_indice(std::string directorioSalida, std::string fileNamee);
        virtual int agregar_documento(std::string documento, int IDdoc);
        virtual int borrar_documento(int IDdoc);
        virtual int borrar_indice();

    protected:  //Esto va a depender del indice que armemos
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
};

#endif // INDICEINVERTIDO_H_INCLUDED
