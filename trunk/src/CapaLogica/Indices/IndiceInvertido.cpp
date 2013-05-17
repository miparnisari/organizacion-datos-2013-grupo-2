#include "IndiceInvertido.h"


IndiceInvertido::IndiceInvertido()
{

}

IndiceInvertido::~IndiceInvertido()
{

}

int IndiceInvertido::crear_indice(std::string directorioSalida, std::string fileNamee)
{
    this->fileName= directorioSalida+fileNamee;
    this->archivo_terminos.crear_archivo(this->fileName+"Terminos");
    this->vocabulario.crear(this->fileName+"Vocabulario", BLOQUE_TAM_DEFAULT);
    this->listas_invertidas.crear(this->fileName+"ListasInvertidas", BLOQUE_TAM_DEFAULT);
    this->archivo_coincidencias.crear_archivo(this->fileName+"Coincidencias");
    return RES_OK;
}

int IndiceInvertido::abrir_indice(std::string directorioSalida, std::string fileName)
{
    int resultado=0;
    resultado = resultado + this->fileName= directorioSalida+fileNamee;
    resultado = resultado + this->archivo_terminos.abrir_archivo(this->fileName+"Terminos");
    resultado = resultado + this->vocabulario.abrir(this->fileName+"Vocabulario");
    resultado = resultado + this->listas_invertidas.abrir(this->fileName+"ListasInvertidas");
    resultado = resultado + this->archivo_coincidencias.abrir_archivo(this->fileName+"Coincidencias");
    if (resultado != 0) return NO_EXISTE_INDICE;
    return RES_OK;
}

int IndiceInvertido::agregar_documento(std::string doc, int IDdoc)
{
    /*Primero pasamos el documento para eliminar los stopwords y nos queda el documento para guardar
    y suponemos que nos devuelve la direccion de un archivo secuencial de registros variables*/

    //Guardamos los terminos y armos las listas invertidas
    this->armar_archivo_coincidencias(documento, IDdoc);
    this->armar_listas_invertidas(documento, IDdoc);
}

int IndiceInvertido::borrar_documento(int IDdoc)
{

}

int IndiceInvertido::borrar_indice()
{
    this->archivo_terminos.eliminar_archivo(this->fileName+"Terminos");
    this->vocabulario.eliminar(this->fileName+"Vocabulario");
    this->listas_invertidas.eliminar(this->fileName+"ListasInvertidas");
    this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias");
}

int IndiceInvertido::armar_listas_invertidas(std::string doc, int IDdoc)
{
    //Lo van a implementar cada indice invertido
}

int IndiceInvertido::armar_archivo_coincidencias(std::string doc, int IDdoc)
{
    //Lo van a implementar cada indice invertido
}
