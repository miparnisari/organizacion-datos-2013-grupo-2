#include "IndiceInvertidoPorTitulo.h"

IndiceInvertidoPorTitulo::IndiceInvertidoPorTitulo()
{

}

IndiceInvertidoPorTitulo::~IndiceInvertidoPorTitulo()
{

}

int IndiceInvertidoPorTitulo::crear_indice(std::string directorioSalida)
{
    this->ruta = directorioSalida;
    int resultado1 = this->indice.crear_archivo(this->ruta+"IndicePorTitulo.dat");
    int resultado2 = this->listas.crear(this->ruta, "ListasPorTitulo.dat");
    if (resultado1 == RES_OK && resultado2 == RES_OK)
    	return RES_OK;
    return RES_ERROR;
}

int IndiceInvertidoPorTitulo::abrir_indice(std::string directorioSalida)
{
    this->ruta = directorioSalida;
    int resultado = this->indice.abrir_archivo(this->ruta+"IndicePorTitulo.dat");
    resultado += this->listas.abrir(this->ruta,"ListasPorTitulo.dat");
    if (resultado != RES_OK)
    	return NO_EXISTE;
    return RES_OK;
}

int IndiceInvertidoPorTitulo::agregar_cancion(RegistroCancion cancion, int IDcancion)
{
    unsigned short ref_lista = this->listas.get_cantidad_listas();
    unsigned short ref_listas[1];
    RegistroClave reg_cancion;
    ClaveX clave;
    clave.set_clave(cancion.get_titulo());
    //Veo si el titulo se encuentra en el indice
    if(this->indice.devolver(clave, &reg_cancion) == NO_EXISTE){
        //Creo un registro con el titulo en el indice y la referencia a una nueva lista
        reg_cancion.set_clave(clave);
        reg_cancion.agregar_campo((char*)&ref_lista,sizeof(ref_lista));
        this->indice.agregar(reg_cancion);
    }else{
        reg_cancion.recuperar_campo((char*)&ref_lista, 0);  /***ver si los campos se guardan desde el 0 o el 1**/
    }
    //Actualizamos la lista del titulo agregandole el IDcancion
    ref_listas[0]= ref_lista;
    return this->listas.recontruir_listas(ref_listas, 1, IDcancion);
}

long IndiceInvertidoPorTitulo::buscar_titulo(std::string titulo, RegistroVariable &listaDeCanciones)
{
    unsigned short ref_lista;
    RegistroClave reg_cancion;
    ClaveX clave;
    clave.set_clave(titulo);
    //Veo si el titulo se encuentra en el indice
    if(this->indice.devolver(clave, &reg_cancion) == NO_EXISTE)
    	return NO_EXISTE;
    //Busco la posicion relativa de la lista en el archivo de listas
    reg_cancion.recuperar_campo((char*)&ref_lista, 0);  /***ver si los campos se guardan desde el 0 o el 1**/
    //Le pido la lista al archivo de listas
    return this->listas.devolver(&listaDeCanciones, ref_lista);
}

int IndiceInvertidoPorTitulo::borrar_indice()
{
    int resultado1= this->indice.eliminar_archivo();
    int resultado2 = this->listas.eliminar(this->ruta,"ListasPorTitulo.dat");
    if (resultado1 == RES_OK && resultado2 == RES_OK)
    	return RES_OK;
    return RES_ERROR;
}
