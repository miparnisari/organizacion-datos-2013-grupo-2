#include "IndiceInvertidoPorAutor.h"

IndiceInvertidoPorAutor::IndiceInvertidoPorAutor()
{

}

IndiceInvertidoPorAutor::~IndiceInvertidoPorAutor()
{

}

int IndiceInvertidoPorAutor::armar_archivo_coincidencias(std::string doc, int IDdoc)
{   //En este caso string doc que se pasa va a ser lo que queremos que se guarde como identificador del documento en el vocabulario
    /**saco el autor del doc y lo agrego en el vobulario*/
/*****    this->agregar_termino(doc, IDdoc, 1); error raro*/
    return RES_OK;
}
