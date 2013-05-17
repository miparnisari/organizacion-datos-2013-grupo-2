#include "IndiceInvertido.h"

IndiceInvertidoPorFrase::IndiceInvertidoPorFrase()
{

}

IndiceInvertidoPorFrase::~IndiceInvertidoPorFrase()
{

}

/**no necesitamos la posicion del id doc*/
int IndiceInvertidoPorFrase::armar_archivo_coincidencias(std::string doc, int IDdoc)
{
/*    ManejadorRegistrosVariables documento;
    RegistroVariable reg, regCoincidencia;
    RegistroClave regClave, regVocabulario;
    ClaveX clave;
    char *campo;
    string termino;
    int i, posNodoSecuencial, IDter;
    documento.abrir_archivo(doc); /*en realidad es la direccion del documento modificado*/
    //Primero guardamos los terminos y armos las listas invertidas
/*    for(i=0; i<documento.get_cantidad_registros_ocupados(); i++){
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
            this->archivo_terminos.agregar_registro(reg);   //Agrego el termino al archivo de terminos
            regVocabulario.set_clave(clave);
            regVocabulario.agregar_campo(IDter, sizeof(IDter));
            regVocabulario.agregar_campo(REF_NULA, sizeof(REF_NULA)); //Seria el numero de la lista dentro del archivo de listas invertida
            this->vocabulario.agregar(regVocabulario);  //Agrego el nuevo termino al vocabulario
        }else{
            /***Busco el IDtermino dentro del vocabulario*/
 /*       }
        regCoincidencia.agregar_campo(IDter,sizeof(IDter));
 /*****       regCoincidencia.agregar_campo(IDdoc,sizeof(IDdoc));   no lo necesito ya que todos son el mismo doc*/
/*        regCoincidencia.agregar_campo(i,sizeof(i)); //Posicion dentro del documento
        this->archivo_coincidencias(regCoincidencia);
    }*/
    return RES_OK;
}

int IndiceInvertidoPorFrase::armar_listas_invertidas()
{

}

int IndiceInvertidoPorFrase::armar_listas_posiciones()
{
/*    int IDter, i, pos, posLista, IDterLista=-1; //ver el -1
    RegistroVariable reg_coin, regPos;
    Bloque *lista;
    char *campo;
    //Ordenamos por IDter y posicion
    /******cuando tengamos el sort lo hacemos**/
    //Vamos armando la lista de posiciones por IDter
/*    for(i=0; i<archivo_coincidencias.get_cantidad_registros_ocupados(); i++){
        //Saco el idtermino del registro
        archivo_coincidencias.get_registro_ocupado(&reg_coin , i);
        reg_coin.recuperar_campo(IDter, 0); //Saco el IDter
        if((IDterLista != -1)&&(IDter != IDterLista)){
            //Obtenemos la posicion de la lista en el archivo de listas
            posLista = buscar_lista (IDterLista);
            //Guardo la lista en el archivo
            archivo_listas_pos.sobreescribir_bloque(lista, posLista);
            //Obtenemos una lista nueva
            lista = archivo_listas_pos.crear_bloque();
        }
        reg_coin.recuperar_campo(campo, 1); /******la posicion del campo posicion*/
/*        pos = atoi(campo);
        regPos.agregar_campo(pos, sizeof(pos));
        lista->agregar_registro(regPos);
    }*/
    return RES_OK;
}

int IndiceInvertidoPorFrase::buscar_lista_pos(int IDterLista)
{
    //Devolvemos la posicion de la lista dentro del archivo de listas de posiciones
/*    RegistroClave reg;
    ClaveX clave;
    char *termino, *refLista;
    int num;
    /*****busco el termino en el archivo de terminos, busqueda binaria*/
/*    clave.set_clave(termino);
    reg.set_clave(clave);
    vocabulario.buscar(reg);
    reg.recuperar_campo(refLista,2);
    num = atoi(refLista);   //recupero el numero
    if(num<0){
        //Modifico el registro para que guarde como referencia a la lista el primer bloque libre
        num = archivo_listas_pos.get_primer_bloque_libre();
        reg.limpiar_campos();
        reg.reg.set_clave(clave);
        reg.agregar_campo(IDterLista, sizeof(IDterLista));
        reg.agregar_campo(IDterLista, sizeof(IDterLista));
    }
    return num;*/
    return RES_OK;
}
