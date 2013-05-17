#ifndef INDICEINVERTIDOPORFRASE_H_INCLUDED
#define INDICEINVERTIDOPORFRASE_H_INCLUDED

#include "IndiceInvertido.h"

class IndiceInvertidoPorFrase : public IndiceInvertido
{

    public:
        IndiceInvertidoPorFrase();
        virtual ~IndiceInvertidoPorFrase();

    private:
        virtual int buscar_lista(int IDterLista, Bloque* lista);
        virtual int armar_listas_posiciones();

    protected:
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);

};


#endif // INDICEINVERTIDOPORFRASE_H_INCLUDED
