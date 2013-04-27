#include "../HashingExtensible/Bloques.h"

Bloques::Bloques(int tamBloque, int bloquesReservados, std::string fileName)
{
   /* this.tamBloque = tamBloque;
    bloquesLibres.Lista();
    cant =0;
    strcpy(this.fileName,fileName);
    this.bloquesReservados=bloquesReservados+1; //El +1 es porque dentro del bloque que le sigue a los reservados vamos a guardar los datos necesarios para usar los bloques
    */
}

 Bloques::~Bloques()
{

}

 int Bloques::agregar_bloque(Bloque bloque){
    int posBloque;
    /*ofstream archivo (fileName);
    if (bloquesLibres.estaVacia()){
        archivo.seekg((cant+bloquesReservados)*tamBloque, ios::beg); //pongo el puntero de escritura donde voy a guardar mi bloque
        posBloque = cant;
        cant ++;
    }else{
        posBloque= bloquesLibres.devolver(1); //le pido que me devuelva la primera pos
        archivo.seekg(posBloque, ios::beg); //pongo el puntero de escritura donde voy a guardar mi bloque
        bloquesLibres.eliminar(1);
    }
    archivo << bloque; //********aca es donde guardo el bloque en el archivo, ver como hacerlo
    //debo ver que si no hay mas espacio devuelva un error*/
    return posBloque;
}

 int Bloques::devolver_bloque(int posicion, Bloque *bloqueDestino)
{
    /*Bloque bloque;
    ifstream archivo (fileName);
    if((posicion>=cant) || (bloquesLibres.contiene(posicion) == FALSO)) return NO_EXISTE;
    archivo.seekp((posicion+bloquesReservados)*tamBloque, ios::beg);
    //aca debo hidratar los datos para devolver en el objeto bloque
    memcpy(bloqueDestino, bloque);
    */
    return RES_OK;
}

 int Bloques::eliminar_bloque(int posicion)
{
    /*bloquesLibres.agregar(posicion);*/
    return RES_OK;
}
