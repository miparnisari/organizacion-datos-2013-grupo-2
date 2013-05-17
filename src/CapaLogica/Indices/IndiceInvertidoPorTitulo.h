#ifndef INDICEINVERTIDOPORTITULO_H_INCLUDED
#define INDICEINVERTIDOPORTITULO_H_INCLUDED

#include "IndiceInvertidoListaConDoc.h"

class IndiceInvertidoPorTitulo : public IndiceInvertidoListaConDoc
{

    public:
        IndiceInvertidoPorTitulo();
        virtual ~IndiceInvertidoPorTitulo();

    protected:
        std::string fileName;
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
};

#endif // INDICEINVERTIDOPORTITULO_H_INCLUDED
