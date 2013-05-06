#include "../HashingExtensible/HashingExtensible.h"


HashingExtensible::HashingExtensible(){}

/**************Recordar que tabla va a tener que extender de registro variable
o hacer algo para poder guardarlo en un bloque, al igual que los registros del hashing

Diagrama de bloques en el archivo:
0: tabla
1..*: bloques de registros

No se necesita guardar nada del Hash porque todo se puede recuperar con el constructor

Falta modificar tabla cuando se elimina un bloque y cuando se agrega: eliminar_registro*/

HashingExtensible::HashingExtensible(std::string directorioSalida, std::string fileNamee)
{
    Bloque bloqueNuevo;
    Tabla tabla;

    //Creo el archivo
    fileName=directorioSalida+fileNamee;
    if (manejador_bloques.abrir_archivo(fileName, "r+") != RES_OK){ //Si no existe es porque estamos creando un nuevo Hash
        manejador_bloques.crear_archivo(fileName, BLOQUE_TAM_DEFAULT);
        // manejador_bloques.abrir_archivo(this.fileName, archivo);

        //La tabla sera el primer bloque del hash
        tabla.modificar_pos(0, 0); // la pos 0 de la tabla se le coloca el num de bloque 1
        guardar_tabla(&tabla); //guardo la tabla en el archivo

        crear_bloque(1, &bloqueNuevo); //Creamos un nuevo bloque con dispersion 1
        manejador_bloques.escribir_bloque(&bloqueNuevo);//va a estar en la posicion 1 porque en el 0 esta la tabla
        cant_bloques = 1; //En realidad es la cantidad de bloques con registro
        //Debo guardar los bloques y la tabla
    }else{
        //Como ya existe suponemos que es el archivo del hash
        cant_bloques = manejador_bloques.get_cantidad_bloques();
    }
    tabla.~Tabla();
    bloqueNuevo.~Bloque();
}

HashingExtensible::~HashingExtensible()
{
    manejador_bloques.~ManejadorBloques();
}


int HashingExtensible::funcion_dispersion(int clave)
{
    char* c_tabla = obtener_tabla();
    int tam_tabla;
    Tabla tabla(c_tabla);
    tam_tabla = tabla.tam();
    tabla.~Tabla();
    return (clave%tam_tabla);
}

int HashingExtensible::agregar(RegistroClave reg)
{
  /*  int posBloque, tamDispersion, i, resultado, posBloqueNuevo, posReg, tamTabla;
    Bloque bloque, bloqueNuevo;
    RegistroVariable tamDispBloque;
    ClaveX clave_reg;
    char *valor;
    //Obtenemos la tabla
    char* c_tabla = obtener_tabla();
    Tabla tabla(c_tabla);
    tamTabla = tabla.tam();

    //obtengo el bloque en donde se guardara el registro
    reg.get_clave(&clave_reg);
    posBloque = obtener_bloque(clave_reg, &bloque);

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
            tabla.modificar_pos(posBloque, posBloqueNuevo);

            //Creamos el bloque con el tam de dispersion del tamaño de la tabla
            crear_bloque(tamTabla, &bloqueNuevo);

        }else{

            tamDispersion = 2*tamDispersion;
            //Modifico en la tabla las posicion por cantidad de tamDispersion
           /******* for(i=0; i<tamTabla; i=i+tamDispersion){     //revisar***********************
                tabla.cambiar_valor(posBloque+i, posBloqueNuevo);
            }*/
            //Creo un bloque con la dispersion que corresponde
 /*           crear_bloque(tamDispersion, &bloqueNuevo);
        }
        cant_bloques++;
        bloqueNuevo.agregar_registro(&reg);
        manejador_bloques.sobreescribir_bloque(&bloqueNuevo, posBloqueNuevo); //Agrego el bloque al archivo
        guardar_tabla(&tabla); //Actualizamos la tabla del archivo
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
{
 /*   Bloque bloque;
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
    clave_reg.~ClaveX();*/
    return RES_OK;
}

int HashingExtensible::eliminar(ClaveX clave)
{
    int posBloque, tamDispersion, i, posDer, posIzq, posReg;
    Bloque bloque;
    Tabla tabla;
    char *tam, *c_tabla;
    RegistroVariable tamDisp;
    //Busco el bloque para agregar el elemento
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
        posDer = tabla.mover_der(tamDispersion);
        posIzq = tabla.mover_izq(tamDispersion*2);
        if (posDer == posIzq){
           // Eliminamos el bloque
            Bloque* bloqueVacio = manejador_bloques.crear_bloque();
            manejador_bloques.sobreescribir_bloque(bloqueVacio,posBloque);
            cant_bloques --;
            for(i=posBloque; i<tabla.tam(); i=i+tamDispersion*2){
                //revisar, o sea deberia dar vueltas con la tabla*************************
                //************************************************************************
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
{
    Bloque bloqueTabla;
    RegistroVariable reg_tabla;
    char *c_tabla = (*tabla).devolver_string();
    //Guardo la tabla en un registro variable
    reg_tabla.agregar_campo(c_tabla,sizeof(c_tabla));
    //Guardo la tabla en un bloque
    bloqueTabla.agregar_registro(&reg_tabla);
    //Guardamos el bloque con la tabla en el archivo
    manejador_bloques.sobreescribir_bloque(&bloqueTabla, 0);
    bloqueTabla.~Bloque();
    reg_tabla.~RegistroVariable();
    return RES_OK;
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
{
   /* int i=0,se_encontro = RES_ERROR;
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
