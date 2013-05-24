#include "IndiceInvertidoPorFrase.h"

/*****Vamos a tener las listas invertidas desordenadas*****/

IndiceInvertidoPorFrase::IndiceInvertidoPorFrase()
{

}

IndiceInvertidoPorFrase::~IndiceInvertidoPorFrase()
{

}

/**no necesitamos la posicion del id doc*/
/*int IndiceInvertidoPorFrase::armar_archivo_coincidencias(std::string doc, int IDdoc)
{
    ManejadorRegistrosVariables documento;
    RegistroVariable reg, regCoincidencia;
    RegistroClave regClave, regVocabulario;
    ClaveX clave;
    char *campo ="", *idtermino="", *iterador="";
  //  string termino;
    int i, posNodoSecuencial, IDter;
    std::string docum;
    *Primero pasamos el documento para eliminar los stopwords y nos queda el documento para guardar
    y suponemos que nos devuelve la direccion de un archivo secuencial de registros variables*/
 /*   documento.abrir_archivo(docum); *en realidad es la direccion del documento modificado*/
    //Primero guardamos los terminos y armos las listas invertidas
/*    for(i=0; i<documento.get_cantidad_registros_ocupados(); i++){
        //Obtengo el primer termino
        documento.get_registro_ocupado(&reg,i);
        reg.recuperar_campo(campo,0);
 //       termino = new string(campo);
        //agrego el termino al vocabulario y al archivo de terminos segun corresponda y al archivo de coincidencias
        this->agregar_termino(campo, IDdoc, i);
    }
    return RES_OK;
}

int IndiceInvertidoPorFrase::agregar_coincidencia(int IDdoc, char* idtermino, int pos)
{
    RegistroVariable regCoincidencia;
    char* iterador; //= itoa (pos, iterador, 10); *****chanchada para guardar numero como char***/
 /*   regCoincidencia.agregar_campo(idtermino,sizeof(idtermino));
    regCoincidencia.agregar_campo(iterador,sizeof(iterador)); //Posicion dentro del documento
    this->archivo_coincidencias.agregar_registro(&regCoincidencia);
    return 0;
}

 ***borrar archivo_listas_pos*/
/*
void IndiceInvertidoPorFrase::crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc)
{
    char *doc;// = itoa(IDdoc, doc, 10);
    regInvertido->agregar_campo(doc, sizeof(doc));
    regInvertido->agregar_campo(pos, sizeof(pos));
}

int IndiceInvertidoPorFrase::buscar(char *elem_busqueda, std::string conjunto_iddoc)
{   //Devuelve NO_EXISTE si no encuentra alguna palabra de la frase
    std::string docum;
    RegistroClave regTermino;
    RegistroVariable reg;
    ClaveX clave;
    char *campo="";
    int i=0, seguir=RES_OK, encontro=RES_ERROR;
    ManejadorRegistrosVariables documento;
    *Primero pasamos la frase de elem_busqueda para eliminar los stopwords y nos queda el documento para guardar
    y suponemos que nos devuelve la direccion de un archivo secuencial de registros variables*/
/*    documento.abrir_archivo(docum); *en realidad es la direccion del documento modificado*/
/*    while(seguir == RES_OK){
        documento.get_registro_ocupado(&reg, i);
        reg.recuperar_campo(campo, 0);
        clave.set_clave(campo);
        regTermino.set_clave(clave);
        //Buscamos las listas invertidas de cada termino
     **   encontro = this->vocabulario.buscar(&regTermino); ****revisar cuando tengamos el buscar del arbol creado***/
/*        if (encontro == RES_ERROR)  return NO_EXISTE;
        ****saco todas las listas y las guardo en un nuevo archivo***/
/*        if(i<documento.get_cantidad_registros_ocupados()) i++;
    }
    this->interseccion_listas_invertidas(this->fileName+"ListasInterseccion");
    return this->armar_lista_documentos(conjunto_iddoc, this->fileName+"ListasInterseccion", docum);
}

int IndiceInvertidoPorFrase::armar_lista_documentos(std::string arch_docs, std::string arch_listas, std::string frase)
{
    ****aca comprobamos que este en el orden correcto a la frase en las listas y devolvemos un nuevo archivo de reg var con el nombre pasado por parametro
         si no se encuentra ni un doc entonces se tira NO_EXISTE**/
/*	return 0;
}*/
