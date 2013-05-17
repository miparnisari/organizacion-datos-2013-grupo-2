#ifndef INDICEINVERTIDOPORAUTOR_H_INCLUDED
#define INDICEINVERTIDOPORAUTOR_H_INCLUDED

#include "IndiceInvertido.h"

class IndiceInvertidoPorAutor : public IndiceInvertido
{

    public:
        IndiceInvertidoPorAutor();
        virtual ~IndiceInvertidoPorAutor();

    private:

    protected:
        std::string fileName;
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
};

#endif // INDICEINVERTIDOPORAUTOR_H_INCLUDED
