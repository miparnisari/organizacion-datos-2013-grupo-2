#ifndef INDICEINVERTIDOPORFRASE_H_INCLUDED
#define INDICEINVERTIDOPORFRASE_H_INCLUDED

#include "IndiceInvertido.h"

class IndiceInvertidoPorFrase : public IndiceInvertido
{

    public:
        IndiceInvertidoPorFrase();
        virtual ~IndiceInvertidoPorFrase();

    private:
        virtual int armar_lista_documentos(std::string arch_docs, std::string arch_listas, std::string frase);

    protected:
        std::string fileName;
        virtual void crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc);
        virtual int agregar_coincidencia(int IDdoc, char* idtermino, int pos);
        virtual int armar_archivo_coincidencias(std::string doc, int IDdoc);
        virtual int buscar(char *elem_busqueda, std::string conjunto_iddoc);
};


#endif // INDICEINVERTIDOPORFRASE_H_INCLUDED
