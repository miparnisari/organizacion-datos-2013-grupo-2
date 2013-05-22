#include "IndiceInvertidoPorAutor.h"

/**El archivo de listas se reconstruye cada vez que agregamos una nueva cancion al indice*/


IndiceInvertidoPorAutor::IndiceInvertidoPorAutor()
{

}

IndiceInvertidoPorAutor::~IndiceInvertidoPorAutor()
{

}

int IndiceInvertidoPorAutor::crear_indice(std::string directorioSalida)
{
    int resultado;
    this->ruta = directorioSalida;
    resultado= resultado+this->indice.crear(this->ruta+"IndicePorAutor.dat", BLOQUE_TAM_DEFAULT);
    resultado= resultado+this->listas.crear_archivo(this->ruta+"ListasPorAutor.dat");
    return resultado;
}

int IndiceInvertidoPorAutor::abrir_indice(std::string directorioSalida)
{
    int resultado;
    this->ruta = directorioSalida;
    this->direccion_listas = this->ruta+"ListasPorAutor";
    resultado= resultado+this->indice.abrir(this->ruta+"IndicePorAutor.dat", "rb+");
    resultado= resultado+this->listas.abrir_archivo(this->direccion_listas+".dat");
    if (resultado !=RES_OK) return NO_EXISTE;
}

int IndiceInvertidoPorAutor::agregar_cancion(RegistroCancion cancion, int IDcancion)
{
    std::string autor;
    ClaveX clave;
    RegistroClave regAutor;
    int i, existe, cant_autores = cancion.get_cantidad_autores();
    //Es la posicion relativa de la proxima lista libre en el archivo de listas de autores
    unsigned short listaVacia= this->listas.get_cantidad_registros();
    unsigned short lista_autores[cant_autores];
    for(i=0; i<cant_autores; i++){
        //Obtenemos un autor de la cancion
        autor = cancion.get_autor(i);
        clave.set_clave(autor);
        regAutor.set_clave(clave);
        /*****existe = this->indice.buscar(regAutor);  //Busco el autor en el indice*/
        if(existe == RES_OK){
            //Como existe entonces buscamos la referencia a la lista de ese autor
            int lista;
            /**busco la lista de referencias a del autor*/
            //Agregamos la referencia a la lista para modificar
            lista_autores[i]=lista;
        }else{
            //Como no existe el autor le crearemos una nueva lista y un registro para el indice
            lista_autores[i]=listaVacia;
            //Creamos el registro para el indice con la clave del autor y la referencia a una lista nueva
            regAutor.agregar_campo((char*)&listaVacia,sizeof(listaVacia));
            this->indice.agregar(regAutor);
            listaVacia++;
        }
    }
    //Actualizamos las listas que tenemos agregandole el IDcancion
    return this->recontruir_listas(lista_autores, cant_autores, IDcancion);
}

int IndiceInvertidoPorAutor::recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int IDcancion)
{   /***estoy pensando que no va a haber listas libres por el medio**/
    ManejadorRegistrosVariables listas_nuevas;
    RegistroVariable lista;
    int resultado;
    resultado = listas_nuevas.crear_archivo(this->direccion_listas+"Auxiliar.dat");
    if (resultado != RES_OK) return resultado;
    listas_nuevas.abrir_archivo(this->direccion_listas+"Auxiliar.dat");
    if (resultado != RES_OK) return resultado;
    unsigned short i, j=0, cant_listas = this->listas.get_cantidad_registros_ocupados();
    for(i=0; i<cant_listas; i++){
        //Obtengo la lista
        listas.get_registro_ocupado(&lista, i);
        if(ref_listas[j] == i){
            //I es una de las referencias a las listas que hay que agregarle el IDcancion
            lista.agregar_campo((char*)&IDcancion,sizeof(IDcancion));
            j++;
        }
        listas_nuevas.agregar_registro(&lista);
    }
    while(j<cant_ref){
        //Creo una lista vacia y le agrego el IDcancion
        RegistroVariable lista;
        lista.agregar_campo((char*)&IDcancion,sizeof(IDcancion));
        listas_nuevas.agregar_registro(&lista);
    }
    listas.eliminar_archivo(this->direccion_listas+".dat");
    //Cambiamos el nombre del archivo por el ori¿ginal
   /***** return rename(this->direccion_listas+"Auxiliar.dat",this->direccion_listas+".dat");***/
}

int IndiceInvertidoPorAutor::borrar_indice()
{
    int resultado;
    resultado = this->indice.eliminar(this->ruta+"Indice");
    if (resultado!=RES_OK)  return resultado;
    resultado = this->listas.eliminar_archivo(this->direccion_listas+".dat");
    if (resultado!=RES_OK)  return resultado;
}

long IndiceInvertidoPorAutor::buscar_autor(std::string autor, RegistroVariable &listaDeCanciones)
{
    int existe;
    unsigned short lista;
    RegistroClave reg_autor;
    ClaveX clave;
    clave.set_clave(autor);
    reg_autor.set_clave(clave);
    /****existe = this->indice.buscar(reg_autor);    //Busco el registro del autor*/
    if(existe != RES_OK)    return NO_EXISTE;
    //Recupero la referencia a la lista
    reg_autor.recuperar_campo((char*)&(lista), 0);  /*****Esto funciona segun los test que hizo ines en el hash sobre int***/
  /****  return this->listas.get_registro_ocupado(listaDeCanciones, lista);***/
}
