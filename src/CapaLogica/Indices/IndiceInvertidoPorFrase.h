#ifndef INDICEINVERTIDOPORFRASE_H_INCLUDED
#define INDICEINVERTIDOPORFRASE_H_INCLUDED

#include "IndiceInvertido.h"

class IndiceInvertidoPorFrase : public IndiceInvertido
{
    private:
        ManejadorBloques archivo_listas_pos;

    public:
        IndiceInvertidoPorFrase();
        virtual ~IndiceInvertidoPorFrase();

    private:
        virtual int buscar_lista_pos(int IDterLista);
        int armar_listas_posiciones();

};


#endif // INDICEINVERTIDOPORFRASE_H_INCLUDED
