#ifndef INDICEINVERTIDO_H_INCLUDED
#define INDICEINVERTIDO_H_INCLUDED

#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../../CapaLogica/ManejoArchivos/Texto.h"
#include "ArchivoListas.h"
#include "../../Constantes.h"

class IndiceInvertido
{
    //Atributos
    protected:
        std::string fileName;
        std::string ruta;
        ManejadorRegistrosVariables archivo_terminos;
        ArbolBMas vocabulario;
        ArchivoListas listas_invertidas;
        ArchivoListas listas_posiciones;
        ManejadorRegistrosVariables archivo_coincidencias;

    public:
        IndiceInvertido();
        virtual ~IndiceInvertido();
        virtual int crear_indice(std::string directorioSalida, std::string fileNamee);
        virtual int abrir_indice(std::string directorioSalida, std::string fileNamee);
        virtual int agregar_cancion(RegistroCancion cancion, int IDcancion);
        virtual int borrar_indice();
        virtual int buscar_frase(std::string frase, RegistroVariable &lista);

    protected:
        virtual int interseccion_listas_invertidas(std::string archivo);
     //   virtual void crear_termino_vocabulario(ClaveX clave, char* idtermino);
        //Esto va a depender del indice que armemos
        virtual void crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc);
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
        //este metodo busca el elemento buscado y crea un archivo de registros variables con la direccion pasada por parametro

};

#endif // INDICEINVERTIDO_H_INCLUDED
