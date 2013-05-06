#include "../HashingExtensible/HashingExtensible.h"


HashingExtensible::HashingExtensible(){}

/**No se necesita guardar nada del Hash porque todo se puede recuperar con el constructor*/

HashingExtensible::HashingExtensible(std::string directorioSalida, std::string fileNamee)
{
    Bloque bloqueNuevo;
    Tabla tabla;

    //Creo el archivo
    fileName=directorioSalida+fileNamee;
    fileNameTabla=directorioSalida+fileNamee+"Tabla";
    if (manejador_bloques.abrir_archivo(fileName, "r+") != RES_OK){ //Si no existe es porque estamos creando un nuevo Hash
        manejador_bloques.crear_archivo(fileName, BLOQUE_TAM_DEFAULT);

        /**esto ya no va mas*///La tabla sera el primer bloque del hash
        /**tabla(fileNameTabla);*/
        tabla.cambiar_valor(0, 0); // la pos 0 de la tabla se le coloca el num de bloque 1
        /**tabla.guardar_cambios();     ya que la tabla ya sabe como guardarlo*/
    /**/    guardar_tabla(&tabla); //guardo la tabla en el archivo

        crear_bloque(1, &bloqueNuevo); //Creamos un nuevo bloque con dispersion 1
        /**con los cambios el primer bloque va a estar en la posicion 0*/
        manejador_bloques.escribir_bloque(&bloqueNuevo);//va a estar en la posicion 1 porque en el 0 esta la tabla
        cant_bloques = 1; //En realidad es la cantidad de bloques con registro
        //Debo guardar los bloques y la tabla
    }else{
        //Como ya existe suponemos que es el archivo del hash
        cant_bloques = manejador_bloques.get_cantidad_bloques();
        /**tabla(fileNameTabla);       y ya la guardamos para trabajar*/
    }
    tabla.~Tabla();
    bloqueNuevo.~Bloque();
}

HashingExtensible::~HashingExtensible()
{
    manejador_bloques.~ManejadorBloques();
//    tabla.~Tabla();
}


int HashingExtensible::funcion_dispersion(int clave)
{
    char* c_tabla = obtener_tabla();
    int tam_tabla;
    Tabla tabla(c_tabla);
    tam_tabla = tabla.get_tamanio();
    tabla.~Tabla();
    return (clave%tam_tabla);
}

int HashingExtensible::agregar(RegistroClave reg)
{   /**Este metodo no se puede desbloquear hasta que no este definida la clase registro clave*/
   /* int posBloque, tamDispersion, i, resultado, posBloqueNuevo, posReg, tamTabla, posTabla;
    Bloque bloque, bloqueNuevo, bloqueAux;
    RegistroVariable tamDispBloque;
    ClaveX clave_reg;
    char *valor;
    //Obtenemos la tabla
    char* c_tabla = obtener_tabla();
    Tabla tabla(c_tabla);
    tamTabla = tabla.get_tamanio();

    //obtengo el bloque en donde se guardara el registro
    reg.get_clave(&clave_reg);
    posBloque = obtener_bloque(clave_reg, &bloque);
    posTabla = obtener_posicion_bloque(clave_reg);

    //Verificamos que no exista la clave del registro que queremos guardar
    posReg = obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR)    return YA_EXISTE;

    //Agrego el elemento
    resultado = bloque.agregar_registro(&reg);

    if (resultado == RES_INSUFFICIENT_SPACE){   //Si desborda el bloque

        //Recupero el tamaño de dispersion del bloque
        bloque.recuperar_registro(&tamDispBloque,0);
        tamDispBloque.recuperar_campo(valor,0);
        tamDispersion = (int)valor;

        //la pos del nuevo bloque es el libre
        posBloqueNuevo = manejador_bloques.get_primer_bloque_libre();

        if(tamDispersion == tamTabla){

            //Duplicamos la tabla
            tabla.duplicar_tabla();
            tabla.cambiar_valor(posTabla, posBloqueNuevo);

            //Creamos el bloque con el tam de dispersion del tamaño de la tabla
            crear_bloque(tamTabla*2, &bloqueNuevo);
            crear_bloque(tamTabla*2, &bloqueAux);

        }else{

            //Modifico en la tabla las posicion por cantidad de tamDispersion
/**/     /*      for(i=posTabla; i<tabla.get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                tabla.cambiar_valor(i,posBloqueNuevo);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                tabla.cambiar_valor(i,posBloqueNuevo);
            }
            //Creo un bloque con la dispersion que corresponde
            crear_bloque(tamDispersion*2, &bloqueNuevo);
            crear_bloque(tamDispersion*2, &bloqueAux);
        }
        cant_bloques++;
        //agrego los bloques modificados
        manejador_bloques.sobreescribir_bloque(&bloqueAux, posBloque); //guardamos uno vacion con el tamaño de dispersion que corresponde
        manejador_bloques.sobreescribir_bloque(&bloqueNuevo, posBloqueNuevo);
        guardar_tabla(&tabla); //Actualizamos la tabla del archivo
        agregar_registros_bloques(bloque, reg);
        resultado= RES_OK;
    }
    bloque.~Bloque();
    bloqueNuevo.~Bloque();
    tabla.~Tabla();
    tamDispBloque.~RegistroVariable();
    clave_reg.~ClaveX();
    return resultado;*/
    return RES_OK;
}

int HashingExtensible::create(std::string directorioSalida, std::string fileName)
{
	return RES_OK;
}

int HashingExtensible::devolver(ClaveX clave, RegistroClave *reg)
{
    int posReg;
    Bloque bloque;
    //Obtengo el bloque que buscamos
    obtener_bloque(clave, &bloque);
    //Modifico el elemento del bloque
    posReg = obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)   return NO_EXISTE;
    bloque.recuperar_registro(reg, posReg);
    bloque.~Bloque();
    return RES_OK;
}

int HashingExtensible::modificar(RegistroClave elemN)
{    /**Este metodo no se puede desbloquear hasta que no este definida la clase registro clave*/
  /*  Bloque bloque;
    int i=1, posReg, clave_hash, posBloque;
    ClaveX clave_reg;
    elemN.get_clave(&clave_reg);
    obtener_bloque(clave_reg, &bloque); //obtengo el bloque
    //Busco el registro a modificar
    posReg = obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR) bloque.eliminar_registro(i); //Borro el viejo registro
    bloque.agregar_registro(&elemN);    //Guardo el nuevo registro
    //Modifico el bloque en los bloques
    manejador_bloques.sobreescribir_bloque(&bloque, posBloque);
    bloque.~Bloque();
    clave_reg.~ClaveX();
    return RES_OK;*/
}

int HashingExtensible::eliminar(ClaveX clave)
{
    int posBloque, tamDispersion, i, posDer, posIzq, posReg, posTabla;
    Bloque bloque;
    Tabla tabla;
    char *tam, *c_tabla;
    RegistroVariable tamDisp;
    //Busco el bloque para agregar el elemento
    posTabla = obtener_posicion_bloque(clave);
    posBloque = obtener_bloque(clave, &bloque);
    posReg = obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)    return RES_OK; // No esta la clave en el archivo, es lo mismo que borrarla
    bloque.eliminar_registro(posReg);    //Elimino el registro
   if (bloque.get_cantidad_registros_almacenados() == 1){ // o sea que solo esta el tam dispersion
        //Busco el tamaño de dispersion del bloque
        bloque.recuperar_registro(&tamDisp, 0);
        tamDisp.recuperar_campo(tam, 0);
        tamDispersion = ((int)tam)/2;

        //me muevo tamDispersion para un lado y para el otro de la lista, si son iguales cambio cada tam disp *2 los bloques de la tabla
        c_tabla = obtener_tabla();
        Tabla tabla(c_tabla);
        posDer = tabla.obtener_valor(posTabla+tamDispersion);
        posIzq = tabla.obtener_valor(posTabla-tamDispersion);
        if (posDer == posIzq){
           // Eliminamos el bloque
            Bloque* bloqueVacio = manejador_bloques.crear_bloque();
            manejador_bloques.sobreescribir_bloque(bloqueVacio,posBloque);
            cant_bloques --;
            //reemplazamos en donde estaba la posicion del bloque viejo por la posicion que corresponda
 /**/           for(i=posTabla; i<tabla.get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                tabla.cambiar_valor(i,posDer);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                tabla.cambiar_valor(i,posDer);
            }
            guardar_tabla(&tabla);
        }
    }
    bloque.~Bloque();
    tabla.~Tabla();
    tamDisp.~RegistroVariable();
    return RES_OK;
}

int HashingExtensible::crear_bloque(int tam, Bloque *bloqueNuevo)
{
//Creamos el bloque con el tam de dispersion del tamaño de la tabla
    RegistroVariable tamDispBloque;
    char *tamDisp;
    tamDisp = (char *)tam;
    tamDispBloque.agregar_campo(tamDisp,sizeof(tamDisp));
    (*bloqueNuevo).agregar_registro(&tamDispBloque);
    tamDispBloque.~RegistroVariable();
    return RES_OK;
}

char *HashingExtensible::obtener_tabla()
{
    //Sacamos la tabla del hash y obtenemos el valor del bloque que buscamos
    RegistroVariable regTabla;
    Bloque *bloqueTabla;
    char *c_tabla;
    bloqueTabla = manejador_bloques.obtener_bloque(0); //Saco el bloque de la tabla
    (*bloqueTabla).recuperar_registro(&regTabla,0); //obtenemos la tabla ***************
    regTabla.recuperar_campo(c_tabla, 0);
    regTabla.~RegistroVariable();
    return c_tabla;
}

int HashingExtensible::guardar_tabla(Tabla *tabla)
{   /***Este metodo no va a servir mas ya que se va a encargar la tabla**/
   /* Bloque bloqueTabla;
    RegistroVariable reg_tabla;
    string c_tabla = (*tabla).persistir_tabla();
    //Guardo la tabla en un registro variable
    reg_tabla.agregar_campo(c_tabla,sizeof(c_tabla));
    //Guardo la tabla en un bloque
    bloqueTabla.agregar_registro(&reg_tabla);
    //Guardamos el bloque con la tabla en el archivo
    manejador_bloques.sobreescribir_bloque(&bloqueTabla, 0);
    bloqueTabla.~Bloque();
    reg_tabla.~RegistroVariable();
    return RES_OK;*/
}

int HashingExtensible::obtener_bloque(ClaveX clave, Bloque *bloque)
{
    int posBloque;
    //Obtengo la tabla
    char* c_tabla = obtener_tabla();
    Tabla tabla(c_tabla);
    //Busco el numero de bloque que necesitamos
    posBloque = tabla.obtener_valor(obtener_posicion_bloque(clave));
    //Busco el bloque para agregar el elemento
    bloque = manejador_bloques.obtener_bloque(posBloque);
    tabla.~Tabla();
    return posBloque;
}

int HashingExtensible::obtener_posicion_reg_bloque(ClaveX clave, Bloque bloque)
{   /**Este metodo no se puede desbloquear hasta que no este definida la clase registro clave*/
  /*  int i=0,se_encontro = RES_ERROR;
    ClaveX clave_reg;
    RegistroClave reg;
    while((se_encontro!=RES_OK)&&(i< bloque.get_cantidad_registros_almacenados())){
        bloque.recuperar_registro(&reg, i);
        reg.get_clave(&clave_reg); //Saco la clave del registro
        if(clave_reg == clave) return i;
        i++;
    }
    clave_reg.~ClaveX();
    reg.~RegistroClave();*/
    return RES_ERROR;
}

int HashingExtensible::obtener_posicion_bloque(ClaveX registro)
{   //Se devuelve la clave pasada por la funcion dispersion
    int clave;
    if(registro.get_tipo_clave() == 'CLAVE_STRING'){
        string claveS;
        registro.get_clave(claveS);
        clave = (int)(claveS.data());
    }else{
        registro.get_clave(clave);
    }
    return funcion_dispersion(clave);
}

int HashingExtensible::agregar_registros_bloques(Bloque bloque, RegistroClave reg)
{   //lo que hacemos es volver a insertar todos los registros y en caso que se desborde el agregar ya se encarga de arreglarlo
    RegistroClave regAux;
    int i, cant = bloque.get_cantidad_registros_almacenados();
    for(i=1; i<cant; i++){
        bloque.recuperar_registro(&regAux, i);
        agregar(regAux);
    }
    agregar(reg);
    return RES_OK;
}
