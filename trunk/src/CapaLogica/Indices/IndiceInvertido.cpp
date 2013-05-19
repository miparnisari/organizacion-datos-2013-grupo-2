#include "IndiceInvertido.h"

/******falta implementar borrar_documento(int IDdoc)**/

int IndiceInvertido::crear_indice(std::string directorioSalida, std::string fileNamee)
{
    this->fileName= directorioSalida+fileNamee;
    this->archivo_terminos.crear_archivo(this->fileName+"Terminos");
    this->vocabulario.crear(this->fileName+"Vocabulario", BLOQUE_TAM_DEFAULT);
    this->listas_invertidas.crear_archivo((this->fileName+"ListasInvertidas"), BLOQUE_TAM_DEFAULT);
    this->archivo_coincidencias.crear_archivo(this->fileName+"Coincidencias");
    return RES_OK;
}

int IndiceInvertido::abrir_indice(std::string directorioSalida, std::string fileNamee)
{
    int resultado=0;
    this->fileName= directorioSalida+fileNamee;
    resultado = resultado + this->archivo_terminos.abrir_archivo(this->fileName+"Terminos");
    resultado = resultado + this->vocabulario.abrir(this->fileName+"Vocabulario", "rb+");
    resultado = resultado + this->listas_invertidas.abrir_archivo(this->fileName+"ListasInvertidas", "rb+");
    resultado = resultado + this->archivo_coincidencias.abrir_archivo(this->fileName+"Coincidencias");
    if (resultado != 0) return NO_EXISTE_INDICE;
    return RES_OK;
}

int IndiceInvertido::agregar_documento(std::string doc, int IDdoc)
{
    //Guardamos los terminos y armos las listas invertidas
    this->armar_archivo_coincidencias(doc, IDdoc);
    this->armar_listas_invertidas(IDdoc);
    this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias");
    return RES_OK;
}

int IndiceInvertido::agregar_termino(char* termino, int IDdoc, int pos)
{
    int posNodoSecuencial, IDter;
    char *idtermino;
    RegistroClave regClave;
    ClaveX clave;
    clave.set_clave(termino);
    regClave.set_clave(clave);
    //Vemos si el termino se encuentra en el vocabulario
/******    posNodoSecuencial = this->vocabulario.buscar(regClave);***********/
    if (posNodoSecuencial== RES_ERROR){ /******Si no existe en el buscar del arbol*/
        IDter = this->archivo_terminos.get_cantidad_registros_ocupados();
        idtermino = itoa (IDter, idtermino, 10); /*****chanchada para guardar numero como char***/
        this->crear_termino_vocabulario(clave, idtermino);
    }else{
        /***Busco el IDter dentro del vocabulario*/
        idtermino = itoa (IDter, idtermino, 10); /*****chanchada para guardar numero como char***/
    }
    this->agregar_coincidencia(IDdoc, idtermino, pos);
    return RES_OK;
}

int IndiceInvertido::borrar_documento(int IDdoc)
{
    /********************FALTA*******************/
    return RES_OK;
}

int IndiceInvertido::borrar_indice()
{
    this->archivo_terminos.eliminar_archivo(this->fileName+"Terminos");
    this->vocabulario.eliminar(this->fileName+"Vocabulario");
    this->listas_invertidas.eliminar_archivo(this->fileName+"ListasInvertidas");
    this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias");
    return RES_OK;
}

void IndiceInvertido::crear_termino_vocabulario(ClaveX clave, char* idtermino){
    //Agrega el nuevo termino al vocabulario con una referencia nula a la lista invertida
    RegistroVariable reg;
    RegistroClave regVocabulario;
    this->archivo_terminos.agregar_registro(&reg);   //Agrego el termino al archivo de terminos
    regVocabulario.set_clave(clave);
    regVocabulario.agregar_campo(idtermino, sizeof(idtermino));
    regVocabulario.agregar_campo(REFE_NULA, sizeof(REFE_NULA)); //Seria el numero de la lista dentro del archivo de listas invertida
    this->vocabulario.agregar(regVocabulario);  //Agrego el nuevo termino al vocabulario
}

int IndiceInvertido::armar_listas_invertidas(int IDdoc)
{
    int IDter, i, posLista, IDterLista=-1; //ver el -1
    RegistroVariable reg_coin, regInvertido;
    Bloque *lista;
    char *campo, *idtermino;
    //Ordenamos por IDter y posicion
    /******cuando tengamos el sort lo hacemos**/
    //Vamos armando la lista de posiciones por IDter
    for(i=0; i<archivo_coincidencias.get_cantidad_registros_ocupados(); i++){
        //Saco el idtermino del registro
        archivo_coincidencias.get_registro_ocupado(&reg_coin , i);
        //Saco el IDter
        reg_coin.recuperar_campo(idtermino, 0);
        IDter = atoi(idtermino);
        if(IDter != IDterLista){
            //Guardo la lista en el archivo
            if (IDterLista!=-1) listas_invertidas.sobreescribir_bloque(lista, posLista);
            //Obtenemos la lista y su posicion en el archivo de listas invertidas
            posLista = this->buscar_lista(IDterLista, lista);
        }
        reg_coin.recuperar_campo(campo, 1); /******la posicion del campo posicion*/
        //Guardo en el registro invertido el idDoc y la posicion
        this->crear_reg_lista_inverida(&regInvertido, campo, IDdoc);
        lista->agregar_registro(&regInvertido);
    }
    return RES_OK;
}

int IndiceInvertido::buscar_lista(int IDterLista, Bloque *lista)
{
    //Devolvemos la posicion de la lista dentro del archivo de listas invertidas y la lista que corresponde
    RegistroClave reg;
    ClaveX clave;
    char *termino, *refLista, *idtermino = itoa (IDterLista, idtermino, 10); /*****chanchada para guardar numero como char***/
    int num;
    /*****busco el termino en el archivo de terminos, busqueda binaria*/
    clave.set_clave(termino);
    reg.set_clave(clave);
/*********    vocabulario.buscar(reg);**********/
    reg.recuperar_campo(refLista,2);
    num = atoi(refLista);   //recupero el numero
    if(num<0){
        //Modifico el registro para que guarde como referencia a la lista el primer bloque libre
        num = listas_invertidas.get_primer_bloque_libre();
        reg.limpiar_campos();
        reg.set_clave(clave);   //Guardo el termino
        reg.agregar_campo(idtermino, sizeof(idtermino));  //Guardo el IDtermino
        reg.agregar_campo(refLista, sizeof(refLista));  //Guardo la referencia a la lista
        lista = listas_invertidas.crear_bloque();
    }else{
        lista = listas_invertidas.obtener_bloque(num);
    }
    return num;
}

int IndiceInvertido::interseccion_listas_invertidas(std::string archivo)
{
    /***aca buscamos la interseccion de estas listas por IDdoc y guardamos las listas en un nuevo archivo de reg variables con el nombre pasado por parametro**/
    return RES_OK;
}

int IndiceInvertido::armar_archivo_coincidencias(std::string doc, int IDdoc)
{
    //Lo van a implementar cada indice invertido
    return RES_OK;
}

int IndiceInvertido::buscar(char *elem_busqueda, std::string conjunto_iddoc)
{
    //Lo van a implementar cada indice invertido
    return RES_OK;
}

int IndiceInvertido::agregar_coincidencia(int IDdoc, char* idtermino, int pos)
{
    //Lo van a implementar cada indice invertido
    return RES_OK;
}

void IndiceInvertido::crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc)
{
    //Lo van a implementar cada indice invertido
}
