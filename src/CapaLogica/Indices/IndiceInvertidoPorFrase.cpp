#include "IndiceInvertidoPorFrase.h"

/*****Vamos a tener las listas invertidas desordenadas*****/

IndiceInvertidoPorFrase::IndiceInvertidoPorFrase()
{

}

IndiceInvertidoPorFrase::~IndiceInvertidoPorFrase()
{

}

/**no necesitamos la posicion del id doc*/
int IndiceInvertidoPorFrase::armar_archivo_coincidencias(std::string doc, int IDdoc)
{
    ManejadorRegistrosVariables documento;
    RegistroVariable reg, regCoincidencia;
    RegistroClave regClave, regVocabulario;
    ClaveX clave;
    char *campo, *idtermino, *iterador;
    string termino;
    int i, posNodoSecuencial, IDter;
    documento.abrir_archivo(doc); /*en realidad es la direccion del documento modificado*/
    //Primero guardamos los terminos y armos las listas invertidas
    for(i=0; i<documento.get_cantidad_registros_ocupados(); i++){
        //Obtengo el primer termino
        documento.get_registro_ocupado(&reg,i);
        reg.recuperar_campo(campo,0);
 //       termino = new string(campo);
        clave.set_clave(campo);
        regClave.set_clave(clave);
        //Vemos si el termino se encuentra en el vocabulario
        posNodoSecuencial = this->vocabulario.buscar(regClave);
        if (posNodoSecuencial== RES_ERROR){ // o sea que no se encontro ese termino en el vocabulario
            IDter = this->archivo_terminos.get_cantidad_registros_ocupados();
            this->archivo_terminos.agregar_registro(&reg);   //Agrego el termino al archivo de terminos
            regVocabulario.set_clave(clave);
            idtermino = itoa (IDter, idtermino, 10); /*****chanchada para guardar numero como char***/
            regVocabulario.agregar_campo(idtermino, sizeof(idtermino));
            regVocabulario.agregar_campo(REF_NULA, sizeof(REF_NULA)); //Seria el numero de la lista dentro del archivo de listas invertida
            this->vocabulario.agregar(regVocabulario);  //Agrego el nuevo termino al vocabulario
        }else{
            /***Busco el IDter dentro del vocabulario*/
            idtermino = itoa (IDter, idtermino, 10); /*****chanchada para guardar numero como char***/
        }
        regCoincidencia.agregar_campo(idtermino,sizeof(idtermino));
        iterador = itoa (i, iterador, 10); /*****chanchada para guardar numero como char***/
        regCoincidencia.agregar_campo(iterador,sizeof(iterador)); //Posicion dentro del documento
        this->archivo_coincidencias.agregar_registro(&regCoincidencia);
    }
    return RES_OK;
}
 /***borrar archivo_listas_pos*/

int IndiceInvertidoPorFrase::armar_listas_invertidas(int IDdoc)
{
    int IDter, i, pos, posLista, IDterLista=-1; //ver el -1
    RegistroVariable reg_coin, regInvertido;
    Bloque *lista;
    char *campo, *doc, *idtermino;
    doc = itoa (IDdoc, doc, 10);
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
            posLista = buscar_lista(IDterLista, lista);
        }
        reg_coin.recuperar_campo(campo, 1); /******la posicion del campo posicion*/
        //Guardo en el registro invertido el idDoc y la posicion
        regInvertido.agregar_campo(doc, sizeof(doc));
        regInvertido.agregar_campo(campo, sizeof(campo));
        lista->agregar_registro(&regInvertido);
    }
    return RES_OK;
}

int IndiceInvertidoPorFrase::buscar_lista(int IDterLista, Bloque *lista)
{
    //Devolvemos la posicion de la lista dentro del archivo de listas invertidas y la lista que corresponde
    RegistroClave reg;
    ClaveX clave;
    char *termino, *refLista, *idtermino = itoa (IDterLista, idtermino, 10); /*****chanchada para guardar numero como char***/
    int num;
    /*****busco el termino en el archivo de terminos, busqueda binaria*/
    clave.set_clave(termino);
    reg.set_clave(clave);
    vocabulario.buscar(reg);
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
