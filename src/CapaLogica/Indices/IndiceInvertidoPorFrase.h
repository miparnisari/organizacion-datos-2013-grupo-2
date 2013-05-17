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
        virtual int interseccion_listas_invertidas(std::string archivo);
        virtual int armar_lista_documentos(std::string arch_docs, std::string arch_listas, std::string frase);

    protected:
        std::string fileName;
        virtual int armar_listas_invertidas(int IDdoc);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
};


#endif // INDICEINVERTIDOPORFRASE_H_INCLUDED
