#ifndef INDICEINVERTIDOPORAUTOR_H_INCLUDED
#define INDICEINVERTIDOPORAUTOR_H_INCLUDED

#include "IndiceInvertidoListaConDoc.h"

class IndiceInvertidoPorAutor : public IndiceInvertidoListaConDoc
{

    public:
        IndiceInvertidoPorAutor();
        virtual ~IndiceInvertidoPorAutor();

    protected:
        std::string fileName;
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
};

#endif // INDICEINVERTIDOPORAUTOR_H_INCLUDED
