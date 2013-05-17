#include "IndiceInvertidoListaConDoc.h"

IndiceInvertidoListaConDoc::IndiceInvertidoListaConDoc()
{

}

IndiceInvertidoListaConDoc::~IndiceInvertidoListaConDoc()
{

}

int IndiceInvertidoListaConDoc::armar_archivo_coincidencias(std::string doc, int IDdoc)
{   //En este caso string doc que se pasa va a ser lo que queremos que se guarde como identificador del documento en el vocabulario
/*****    this->agregar_termino(doc, IDdoc, 1); error raro*/
}

int IndiceInvertidoListaConDoc::agregar_coincidencia(int IDdoc, char* idtermino, int pos)
{
    RegistroVariable regCoincidencia;
    regCoincidencia.agregar_campo(idtermino,sizeof(idtermino));
    this->archivo_coincidencias.agregar_registro(&regCoincidencia);
}

void IndiceInvertidoListaConDoc::crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc)
{
    char *doc = itoa(IDdoc, doc, 10);
    regInvertido->agregar_campo(doc, sizeof(doc));
}

int IndiceInvertidoListaConDoc::buscar(char *elem_busqueda, std::string conjunto_iddoc)
{   //Devuelve NO_EXISTE si no encuentra alguna palabra en el vocabulario
    RegistroClave regTermino;
    RegistroVariable reg;
    ClaveX clave;
    Bloque *lista;
    char *campo;
    int encontro, i;
    ManejadorRegistrosVariables documento;
    clave.set_clave(elem_busqueda);
    regTermino.set_clave(clave);
    //Buscamos las listas invertidas de cada termino
     /**   encontro = this->vocabulario.buscar(&regTermino); /****revisar cuando tengamos el buscar del arbol creado***/
    if (encontro == RES_ERROR)  return NO_EXISTE;
    //Guardo la lista en cinjunto_iddoc
    lista = listas_invertidas.obtener_bloque(encontro); //saco la lista de iddoc
    documento.crear_archivo(conjunto_iddoc);
    documento.abrir_archivo(conjunto_iddoc);
    for(i=0; i<lista->get_cantidad_registros_almacenados(); i++){
        lista->recuperar_registro(&reg, i);
        documento.agregar_registro(&reg);
    }
    return RES_OK;
}
