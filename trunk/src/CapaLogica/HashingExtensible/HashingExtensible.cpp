#include "../HashingExtensible/HashingExtensible.h"

HashingExtensible::HashingExtensible(){}

HashingExtensible::HashingExtensible(std::string directorioSalida, std::string fileName)
{
    /*//podria crear ya un bloque y una posicion en la tabla para iniciar.....***************
    Bloque bloqueNuevo;

    //Creo el archivo
    strcpy(this.fileName,fileName);
    ofstream arch_escr(fileName);

    tabla.Tabla();
    tabla.modificar_pos(0, 0); // la pos 0 de la tabla se le coloca el num de bloque 0
    bloqueNuevo.Bloque(1, 0); //tam dispersion, num de bloque
    bloques.Bloques(TAM_BLOQUE, 1, fileName); //1 es porque el primero es una tabla, TAM_BLOQUE aun no lo se--- como lo saco??********
    bloques.agregar(bloqueNuevo);
    cantBloques = 1;*/
    //Debo guardar los bloques y la tabla
}

/*Al eliminar un bloque no hay que borrarlo sino hay que ponerlo como vacio y guardar la
posicion en bloques_vacios para poder reutilizarlo para no desorganizar*/

HashingExtensible::~HashingExtensible()
{

}

/*
int Hash::funcion_dispersion(int clave){
    return (clave%tabla.tam());
}*/

int HashingExtensible::agregar(RegistroVariable& reg)
{
    /*int dispersion = elem.devolver_clave();
    dispersion = this.funcion_dispersion(dispersion);
    int posBloque = tabla.obtener_valor(dispercion);
    int tamDispersion, i, resultado, posBloqueNuevo;
    Bloque bloque, bloqueNuevo;
    //Busco el bloque para agregar el elemento
    bloque = bloques.devolver_bloque(posBloque);
    //Agrego el elemento
    resultado = bloque.agregar_elem(elem);
    if (resultado == DESBORDO){
        tamDispersion = bloque.tamDisp();
        //la pos del nuevo bloque es el libre
        posBloqueNuevo = bloques_libres.devolver(0);
        if(tamDispersion == tabla.tam()){
            //Duplicamos la tabla
            tabla.duplicar_tabla();
            tabla.cambiar_valor(posBloque, cant_bloques);
            //Creamos el bloque
            bloqueNuevo.bloque(tabla.tam(), cant_bloques);
        }else{
            tamDispersion = 2*tamDispersion;
            //Modifico en la tabla las posicion por cantidad de tamDispersion
            for(i=0; i<tabla.tam(); i=i+tamDispersion){     //revisar***********************
                tabla.cambiar_valor(posBloque+i, cant_bloques);
            }
            bloqueNuevo.bloque(tamDispersion);
        }
        cant_bloques++;
        bloqueNuevo.agregar_elem(elem);
        //Agrego el bloque al listado
        bloques.agregar(bloqueNuevo);
        resutado= RES_OK;
    }
    return resultado;*/
	return RES_OK;
}

int HashingExtensible::create(std::string directorioSalida, std::string fileName)
{
	return RES_OK;
}

RegistroVariable HashingExtensible::devolver(int clave){
    RegistroVariable reg;
  /*  int posBloque = tabla.obtener_valor(this.funcion_dispersion(clave);
    Bloque bloque;
    //Busco el bloque para agregar el elemento
    bloque = bloques.devolver_bloque(posBloque);
    //Modifico el elemento del bloque
    return bloque.devolver(clave);*/
    return reg;
}

int HashingExtensible::modificar(int clave, RegistroVariable elemN){
  /*  int posBloque = tabla.obtener_valor(this.funcion_dispersion(clave);
    Bloque bloque;
    //Busco el bloque para agregar el elemento
    bloque = bloques.devolver_bloque(posBloque);
    //Modifico el elemento del bloque
    bloque.modificar(clave, elemN);
    //Modifico el bloque en los bloques
    return bloques.modificar(posBloque, bloque);*/
    return RES_OK;
}

int HashingExtensible::eliminar(int clave){
   /* int posBloque = tabla.obtener_valor(this.funcion_dispersion(clave);
    int tamDispersion, i, posDer, posIzq;
    Bloque bloque;
    //Busco el bloque para agregar el elemento
    bloque = bloques.devolver_bloque(posBloque);
    if(bloque.eliminar_elem(clave) == ESTA_VACIO){
        tamDispersion = bloque.tamDisp()/2;
        //me muevo tamDispersion para un lado y para el otro de la lista, si son iguales cambio cada tam disp *2 los bloques de la tabla
        posDer = tabla.mover_der(tamDispersion);
        posIzq = tabla.mover_izq(tamDispersion*2);
        if (posDer == posIzq){
            bloques.eliminar(posBloque);
            cant_bloques --;
            //guardo el bloque en los libres
            bloques_libre.agregar(posBloque);
            for(i=posBloque; i<tabla.tam(); i=i+tamDispersion*2){
                //revisar, o sea deberia dar vueltas con la tabla*************************
            }
        }
    }
    return RES_OK;*/
    return RES_OK;
}


int HashingExtensible::funcion_dispersion(int clave){return 0;}

