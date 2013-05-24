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
    resultado= this->indice.crear(this->ruta+"IndicePorAutor.dat", BLOQUE_TAM_DEFAULT);
    resultado= resultado+this->listas.crear(this->ruta,"ListasPorAutor.dat");
    return resultado;
}

int IndiceInvertidoPorAutor::abrir_indice(std::string directorioSalida)
{
    int resultado;
    this->ruta = directorioSalida;
    resultado= this->indice.abrir(this->ruta+"IndicePorAutor.dat", "rb+");
    resultado= resultado+this->listas.abrir(this->ruta, "ListasPorAutor.dat");
    if (resultado !=RES_OK) return NO_EXISTE;
    return resultado;
}

int IndiceInvertidoPorAutor::agregar_cancion(RegistroCancion cancion, int IDcancion)
{
    std::string autor;
    ClaveX clave;
    RegistroClave regAutor;
    int i, existe;
    const int cant_autores = cancion.get_cantidad_autores();
    //Es la posicion relativa de la proxima lista libre en el archivo de listas de autores
    unsigned short listaVacia= this->listas.get_cantidad_listas();
    unsigned short* lista_autores = new unsigned short[cant_autores]();
    for(i=0; i<cant_autores; i++){
        //Obtenemos un autor de la cancion
        autor = cancion.get_autor(i);
        clave.set_clave(autor);
        regAutor.set_clave(clave);
        //Busco el autor en el indice
        existe = this->indice.buscar(regAutor);
        if(existe == RES_OK){
            //Como existe entonces buscamos la referencia a la lista de ese autor
            int lista;
            //busco la referencias a la lista del autor
            regAutor.recuperar_campo((char*)&lista, 0);  /***ver si los campos se guardan desde el 0 o el 1**/
            //Agregamos la referencia a la lista para modificar
            *(lista_autores+i)=lista;
        }else{
            //Como no existe el autor le crearemos una nueva lista y un registro para el indice
            *(lista_autores+i)=listaVacia;
            //Creamos el registro para el indice con la clave del autor y la referencia a una lista nueva
            regAutor.agregar_campo((char*)&listaVacia,sizeof(listaVacia));
            this->indice.agregar(regAutor);
            listaVacia++;
        }
    }
    //Actualizamos las listas que tenemos agregandole el IDcancion
    return this->listas.recontruir_listas(lista_autores, cant_autores, IDcancion);
}

int IndiceInvertidoPorAutor::borrar_indice()
{
    int resultado;
    resultado = this->indice.eliminar(this->ruta+"Indice");
    if (resultado!=RES_OK)  return resultado;
    resultado = this->listas.eliminar(this->ruta,"ListasPorAutor.dat");
    if (resultado!=RES_OK)  return resultado;
    return RES_OK;
}

long IndiceInvertidoPorAutor::buscar_autor(std::string autor, RegistroVariable &listaDeCanciones)
{
    int existe;
    unsigned short lista;
    RegistroClave reg_autor;
    ClaveX clave;
    clave.set_clave(autor);
    reg_autor.set_clave(clave);
    //Busco las canciones del autor
    existe = this->indice.buscar(reg_autor);
    if(existe != RES_OK)    return NO_EXISTE;
    //Recupero la referencia a la lista
    reg_autor.recuperar_campo((char*)&(lista), 0);
    return this->listas.devolver(&listaDeCanciones, lista);
}
