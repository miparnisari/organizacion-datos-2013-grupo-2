#include "../HashingExtensible/HashingExtensible.h"


/**No se necesita guardar nada del Hash porque todo se puede recuperar con el constructor

Falta revisar como se guardan los tamanios de dispersion de los bloques*/

HashingExtensible::HashingExtensible(std::string directorioSalida, std::string fileNamee)
{
    Bloque *bloqueNuevo;

    this->fileName=directorioSalida+fileNamee;
    this->fileNameTabla=directorioSalida+fileNamee+"Tabla";
    if (this->manejador_bloques.abrir_archivo(fileName, "r+") != RES_OK){ //Si no existe es porque estamos creando un nuevo Hash
        this->manejador_bloques.crear_archivo(fileName, BLOQUE_TAM_DEFAULT);
        //Creamos la tabla
        //no es necesario cargar tabla
        //this->tabla.cargar_tabla(fileNameTabla);
        this->tabla.cambiar_valor(fileName,0, 0); // la pos 0 de la tabla se le coloca el num de bloque 0
        //Guardamos los cambios de la tabla
        //saco el persistir tabla, no necesitamos guartdarla
        //this->tabla.persistir_tabla(fileNameTabla);
        //Creamos un nuevo bloque con dispersion 1 y lo guardamos en el archivo de bloques del Hash
        bloqueNuevo = this->manejador_bloques.crear_bloque();
        this->crear_bloque(1, bloqueNuevo);
        this->manejador_bloques.escribir_bloque(bloqueNuevo);//Va a estar en la posicion 0
        this->cant_bloques = 1;
    }else{
        //Ya abrimos el archivo de bloques
        this->cant_bloques = manejador_bloques.get_cantidad_bloques();

        //saco el metodo, pues no necesitamos cargar tabla, esta siempre en archivo
        //this->tabla.cargar_tabla(fileNameTabla);
    }
}

HashingExtensible::~HashingExtensible()
{
    this->manejador_bloques.~ManejadorBloques();
    this->tabla.~Tabla();
}

int HashingExtensible::eliminar_hashing_extensible()
{
    this->manejador_bloques.eliminar_archivo(fileName);
  //  this->tabla.eliminar_tabla();
    return RES_OK;
}

int HashingExtensible::funcion_dispersion(int clave)
{
    int tam_tabla;
    tam_tabla = this->tabla.get_tamanio();
    return (clave%tam_tabla);
}

int HashingExtensible::agregar(RegistroClave reg)
{
    int posBloque, tamDispersion, i, resultado, posBloqueNuevo, posReg, tamTabla, posTabla;
    Bloque bloque;
    RegistroVariable tamDispBloque;
    ClaveX clave_reg;
    char* valor = NULL;

    tamTabla = tabla.get_tamanio();
    //Obtengo el bloque en donde se guardara el registro
    reg.get_clave(clave_reg);
    posBloque = this->obtener_bloque(clave_reg, &bloque);
    posTabla = this->obtener_posicion_tabla(clave_reg);

    //Verificamos que no exista la clave del registro que queremos guardar
    posReg = this->obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR)    return YA_EXISTE;

    //Agrego el elemento
    resultado = bloque.agregar_registro(&reg);

    if (resultado == RES_INSUFFICIENT_SPACE){   //Si desborda el bloque
        Bloque bloqueNuevo(this->manejador_bloques.get_tamanio_bloque());
        Bloque bloqueAux(this->manejador_bloques.get_tamanio_bloque());
        //Recupero el tamaño de dispersion del bloque
        bloque.recuperar_registro(&tamDispBloque,0);
        tamDispBloque.recuperar_campo(valor,0);
        tamDispersion = (int)valor;

        //La pos del nuevo bloque es el libre
        posBloqueNuevo = this->manejador_bloques.get_primer_bloque_libre();

        if(tamDispersion == tamTabla){

            //Duplicamos la tabla
            this->tabla.duplicar_tabla(fileNameTabla);
            this->tabla.cambiar_valor(fileNameTabla,posTabla, posBloqueNuevo);

            //Creamos el bloque con el tam de dispersion del tamanio de la tabla
            this->crear_bloque(tamTabla*2, &bloqueNuevo);
            this->crear_bloque(tamTabla*2, &bloqueAux);

        }else{

            //Modifico en la tabla las posicion por cantidad de tamDispersion
/**/           for(i=posTabla; i<tabla.get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                this->tabla.cambiar_valor(fileNameTabla,i,posBloqueNuevo);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                this->tabla.cambiar_valor(fileNameTabla,i,posBloqueNuevo);
            }
            //Creo un bloque con la dispersion que corresponde
            this->crear_bloque(tamDispersion*2, &bloqueNuevo);
            this->crear_bloque(tamDispersion*2, &bloqueAux);
        }
        cant_bloques++;
        //agrego los bloques vacios con el tamanio de dispersion que corresponde
        this->manejador_bloques.sobreescribir_bloque(&bloqueAux, posBloque);
        this->manejador_bloques.sobreescribir_bloque(&bloqueNuevo, posBloqueNuevo);
        //Actualizamos la tabla del archivo
        //nuevamente, saco persistir tabla
        //this->tabla.persistir_tabla(fileNameTabla);
        //Agrego todos los registros del bloque que desbordo junto con el nuevo registro
        this->agregar_registros_bloques(bloque, reg);
        bloqueNuevo.~Bloque();
        bloqueAux.~Bloque();
        resultado= RES_OK;
    }
    bloque.~Bloque();
    tamDispBloque.~RegistroVariable();
    clave_reg.~ClaveX();
    return resultado;
}

int HashingExtensible::devolver(ClaveX clave, RegistroClave *reg)
{
    int posReg;
    Bloque bloque;
    //Obtengo el bloque que buscamos
    this->obtener_bloque(clave, &bloque);
    //Modifico el elemento del bloque
    posReg = this->obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)   return NO_EXISTE;
    bloque.recuperar_registro(reg, posReg);
    bloque.~Bloque();
    return RES_OK;
}

int HashingExtensible::modificar(RegistroClave elemN)
{
    Bloque bloque;
    int posReg, posBloque;
    ClaveX clave_reg;
    elemN.get_clave(clave_reg);
    //obtengo el bloque que buscamos
    posBloque = this->obtener_bloque(clave_reg, &bloque);
    //Busco el registro a modificar
    posReg = this->obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR) bloque.eliminar_registro(posReg); //Borro el viejo registro
    bloque.agregar_registro(&elemN);    //Guardo el nuevo registro
    //Modifico el bloque en los bloques
    this->manejador_bloques.sobreescribir_bloque(&bloque, posBloque);
    bloque.~Bloque();
    clave_reg.~ClaveX();
    return RES_OK;
}

int HashingExtensible::eliminar(ClaveX clave)
{
    int posBloque, tamDispersion, i, posDer, posIzq, posReg, posTabla;
    Bloque bloque;
    char* tam = NULL;
    RegistroVariable tamDisp;
    //Busco el bloque para agregar el elemento
    posTabla = this->obtener_posicion_tabla(clave);
    posBloque = this->obtener_bloque(clave, &bloque);
    posReg = this->obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)    return RES_OK; // No esta la clave en el archivo, es lo mismo que borrarla
    //Elimino el registro
    bloque.eliminar_registro(posReg);
    if (bloque.get_cantidad_registros_almacenados() == 1){ // o sea que solo esta el tam dispersion
        //Busco el tamanio de dispersion del bloque
        bloque.recuperar_registro(&tamDisp, 0);
        tamDisp.recuperar_campo(tam, 0);
        tamDispersion = ((int)tam)/2;

        //Me muevo tamDispersion para un lado y para el otro de la lista, si son iguales cambio cada tam disp *2 los bloques de la tabla
        posDer = this->tabla.obtener_valor(fileNameTabla, posTabla+tamDispersion);
        posIzq = this->tabla.obtener_valor(fileNameTabla, posTabla-tamDispersion);
        if (posDer == posIzq){
            //Eliminamos el bloque
            Bloque* bloqueVacio = this->manejador_bloques.crear_bloque();
            this->manejador_bloques.sobreescribir_bloque(bloqueVacio,posBloque);
            this->cant_bloques --;
            //Reemplazamos en donde estaba la posicion del bloque viejo por la posicion que corresponda
 /**/           for(i=posTabla; i<tabla.get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                this->tabla.cambiar_valor(fileNameTabla, i,posDer);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                this->tabla.cambiar_valor(fileNameTabla,i,posDer);
            }
            this->dividir_tabla();
            //saco esto, pues ya no hay necesidad de persistir
            //this->tabla.persistir_tabla(fileNameTabla);
        }
    }
    bloque.~Bloque();
    tamDisp.~RegistroVariable();
    return RES_OK;
}

int HashingExtensible::crear_bloque(int tam, Bloque *bloqueNuevo)
{
//Creamos el bloque con el tam de dispersion del tama�o de la tabla
    RegistroVariable tamDispBloque;
    char *tamDisp;
    tamDisp = (char *)tam;
    //Le guardo el tamaño de dispersion
    tamDispBloque.agregar_campo(tamDisp,sizeof(tamDisp));
    bloqueNuevo->agregar_registro(&tamDispBloque);
    tamDispBloque.~RegistroVariable();
    return RES_OK;
}

int HashingExtensible::obtener_bloque(ClaveX clave, Bloque *bloque)
{
    int posBloque;
    //Busco el numero de bloque que necesitamos
    posBloque = this->tabla.obtener_valor(fileNameTabla, this->obtener_posicion_tabla(clave));
    //Busco el bloque para agregar el elemento
    bloque = this->manejador_bloques.obtener_bloque(posBloque);
    return posBloque;
}

int HashingExtensible::obtener_posicion_reg_bloque(ClaveX clave, Bloque bloque)
{
    int i=0,se_encontro = RES_ERROR;
    ClaveX clave_reg;
    RegistroClave reg;
    while((se_encontro!=RES_OK)&&(i< bloque.get_cantidad_registros_almacenados())){
        bloque.recuperar_registro(&reg, i);
        reg.get_clave(clave_reg);
        if(clave_reg == clave) se_encontro=RES_OK;
        i++;
    }
    clave_reg.~ClaveX();
    if (se_encontro ==RES_OK) return i;
    return RES_ERROR;
}

int HashingExtensible::obtener_posicion_tabla(ClaveX registro)
{   //Se devuelve la clave pasada por la funcion dispersion
    int clave;
    if(registro.get_tipo_clave() == CLAVE_STRING){
        string claveS;
        registro.get_clave(claveS);
        clave = (int)(claveS.data());
    }else{
        registro.get_clave(clave);
    }
    return funcion_dispersion(clave);
}

int HashingExtensible::agregar_registros_bloques(Bloque bloque, RegistroClave reg)
{   //Inserta todos los registros y en caso que se desborde el agregar ya se encarga de arreglarlo
    RegistroClave regAux;
    int i, cant = bloque.get_cantidad_registros_almacenados();
    for(i=1; i<cant; i++){
        bloque.recuperar_registro(&regAux, i);
        this->agregar(regAux);
    }
    this->agregar(reg);
    return RES_OK;
}

int HashingExtensible::dividir_tabla()
{
    int i=0, mitad=(this->tabla.get_tamanio())/2, resultado=RES_OK;
    while ((resultado == RES_OK)&&(i < mitad)){
        if((this->tabla.obtener_valor(fileNameTabla, i)) != (this->tabla.obtener_valor(fileNameTabla, (i+mitad))))
            resultado = RES_ERROR;
        i++;
    }
    return RES_OK;
}
