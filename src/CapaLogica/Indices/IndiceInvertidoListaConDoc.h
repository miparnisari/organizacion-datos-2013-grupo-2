#ifndef INDICEINVERTIDOLISTACONDOC_H_INCLUDED
#define INDICEINVERTIDOLISTACONDOC_H_INCLUDED

#include "IndiceInvertido.h"

/**Esta clase va a servir para los indices por titulo y autor*/

class IndiceInvertidoListaConDoc : public IndiceInvertido
{

    public:
        IndiceInvertidoListaConDoc();
        virtual ~IndiceInvertidoListaConDoc();

    protected:
        std::string fileName;
        virtual void crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc);
        virtual int agregar_coincidencia(int IDdoc, char* idtermino, int pos);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
};

#endif // INDICEINVERTIDOLISTACONDOC_H_INCLUDED
