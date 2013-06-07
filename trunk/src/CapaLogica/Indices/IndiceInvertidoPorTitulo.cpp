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
    int res = this->indice.crear_archivo(this->ruta+"IndicePorTitulo");
    res += this->listas.crear(this->ruta, "ListasPorTitulo");
    return res;
}

int IndiceInvertidoPorTitulo::abrir_indice(std::string directorioSalida)
{
    this->ruta = directorioSalida;
    int resultado = this->indice.abrir_archivo(this->ruta+"IndicePorTitulo");
    resultado += this->listas.abrir(this->ruta,"ListasPorTitulo");
    return resultado;
}

int IndiceInvertidoPorTitulo::agregar_cancion(RegistroCancion & cancion, int IDcancion)
{
    unsigned short ref_lista = this->listas.get_cantidad_listas();
    unsigned short* ref_listas = new unsigned short[1]();
    RegistroClave reg_cancion;
    ClaveX clave;
    clave.set_clave(cancion.get_titulo());
    //Veo si el titulo se encuentra en el indice
    if(this->indice.devolver(clave, &reg_cancion) == RES_RECORD_DOESNT_EXIST){
        //Creo un registro con el titulo en el indice y la referencia a una nueva lista
        reg_cancion.set_clave(clave);
        reg_cancion.agregar_campo((char*)&ref_lista,sizeof(ref_lista));
        this->indice.agregar(reg_cancion);
    }else{
        reg_cancion.recuperar_campo((char*)&ref_lista, 1);
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
    if(this->indice.devolver(clave, &reg_cancion) == RES_RECORD_DOESNT_EXIST)
    	return RES_RECORD_DOESNT_EXIST;
    //Busco la posicion relativa de la lista en el archivo de listas
    reg_cancion.recuperar_campo((char*)&ref_lista, 1);
    //Le pido la lista al archivo de listas
    return this->listas.devolver(&listaDeCanciones, ref_lista);
}

int IndiceInvertidoPorTitulo::borrar_indice()
{
    int res = this->indice.eliminar_archivo();
    res += this->listas.eliminar(this->ruta,"ListasPorTitulo");
    return res;
}
