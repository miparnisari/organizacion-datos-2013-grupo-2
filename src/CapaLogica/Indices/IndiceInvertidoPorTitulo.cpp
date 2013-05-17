#include "IndiceInvertidoPorTitulo.h"

IndiceInvertidoPorTitulo::IndiceInvertidoPorTitulo()
{

}

IndiceInvertidoPorTitulo::~IndiceInvertidoPorTitulo()
{

}


int IndiceInvertidoPorTitulo::armar_archivo_coincidencias(std::string doc, int IDdoc)
{   //En este caso string doc que se pasa va a ser lo que queremos que se guarde como identificador del documento en el vocabulario
    /**saco el titulo del doc y lo agrego en el vobulario*/
/*****    this->agregar_termino(doc, IDdoc, 1); error raro*/
    return RES_OK;
}
