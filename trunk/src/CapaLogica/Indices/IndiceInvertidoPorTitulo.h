#ifndef INDICEINVERTIDOPORTITULO_H_INCLUDED
#define INDICEINVERTIDOPORTITULO_H_INCLUDED

#include "IndiceInvertido.h"

class IndiceInvertidoPorTitulo : public IndiceInvertido
{

    public:
        IndiceInvertidoPorTitulo();
        virtual ~IndiceInvertidoPorTitulo();

    private:

    protected:
        std::string fileName;
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
};

#endif // INDICEINVERTIDOPORTITULO_H_INCLUDED
