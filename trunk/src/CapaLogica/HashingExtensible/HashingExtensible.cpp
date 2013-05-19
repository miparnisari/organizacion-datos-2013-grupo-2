#include "../HashingExtensible/HashingExtensible.h"
#include <sstream>

/**No se necesita guardar nada del Hash porque todo se puede recuperar con el constructor

Falta revisar como se guardan los tamanios de dispersion de los bloques*/

HashingExtensible::HashingExtensible(std::string directorioSalida, std::string fileNamee)
{
    this->tabla = NULL;
    this->fileName=directorioSalida+fileNamee;
    this->fileNameTabla=directorioSalida+fileNamee+"Tabla";

    if (this->manejador_bloques.abrir_archivo(fileName, "rb+") != RES_OK){ //Si no existe es porque estamos creando un nuevo Hash
        this->manejador_bloques.crear_archivo(fileName, BLOQUE_TAM_DEFAULT);
        this->manejador_bloques.abrir_archivo(fileName, "rb+");
        //Creamos la tabla
        this->tabla = new Tabla;
        this->tabla->crear(fileNameTabla);
        this->tabla->cambiar_valor(0, 0); // la pos 0 de la tabla se le coloca el num de bloque 0
        //Creamos un nuevo bloque con dispersion 1 y lo guardamos en el archivo de bloques del Hash
        Bloque* bloqueNuevo = this->manejador_bloques.crear_bloque();
        this->crear_bloque(1, bloqueNuevo);
        this->manejador_bloques.escribir_bloque(bloqueNuevo);//Va a estar en la posicion 0
        delete (bloqueNuevo);
        this->cant_bloques = 1;
    }else{
        //Ya abrimos el archivo de bloques
        this->cant_bloques = manejador_bloques.get_cantidad_bloques();
    }
    this->manejador_bloques.cerrar_archivo();
}

HashingExtensible::~HashingExtensible()
{
    delete(tabla);
}

int HashingExtensible::eliminar_hashing_extensible()
{
	int resultado = this->manejador_bloques.eliminar_archivo(this->fileName);
	tabla->eliminar();
	return resultado;

}

int HashingExtensible::funcion_dispersion(int clave)
{
    int tam_tabla;
    tam_tabla = this->tabla->get_tamanio();
    return (clave%tam_tabla);
}

int HashingExtensible::agregar(RegistroClave reg)
{
    int posBloque, tamDispersion, i, resultado, posBloqueNuevo, posReg, tamTabla, posTabla;
    Bloque bloque;
    RegistroVariable tamDispBloque;
    ClaveX clave_reg = reg.get_clave();
 /**   std::string valor;*/

    this->manejador_bloques.abrir_archivo(fileName, "rb+");

    tamTabla = tabla->get_tamanio();
    //Obtengo el bloque en donde se guardara el registro

    posBloque = this->obtener_bloque(clave_reg, &bloque);
    posTabla = this->obtener_posicion_tabla(clave_reg);

    //Verificamos que no exista la clave del registro que queremos guardar
    posReg = this->obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR)    return YA_EXISTE;

    //Agrego el elemento
    resultado = bloque.agregar_registro(&reg);

    if (resultado == RES_INSUFFICIENT_SPACE){   //Si desborda el bloque
        ClaveX valor;   //tamanio de dispersion del bloque
        Bloque bloqueNuevo(this->manejador_bloques.get_tamanio_bloque());
        Bloque bloqueAux(this->manejador_bloques.get_tamanio_bloque());
        //Recupero el tamaño de dispersion del bloque
        bloque.recuperar_registro(&tamDispBloque,0);
        char* buffer = new char[50]();
        tamDispBloque.recuperar_campo(buffer,0);
        valor.desempaquetar(buffer, 5);     /**************como saco el tamanio con que se empaqueto?????****************/
        valor.get_clave(tamDispersion);

        //La pos del nuevo bloque es el libre
        posBloqueNuevo = this->manejador_bloques.get_primer_bloque_libre();

        if(tamDispersion == tamTabla){

            //Duplicamos la tabla
        	this->tabla->duplicar();
            this->tabla->cambiar_valor(posTabla, posBloqueNuevo);

            //Creamos el bloque con el tam de dispersion del tamanio de la tabla
            this->crear_bloque(tamTabla*2, &bloqueNuevo);
            this->crear_bloque(tamTabla*2, &bloqueAux);

        }else{

            //Modifico en la tabla las posicion por cantidad de tamDispersion
/**/           for(i=posTabla; i<tabla->get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                this->tabla->cambiar_valor(i,posBloqueNuevo);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                this->tabla->cambiar_valor(i,posBloqueNuevo);
            }
            //Creo un bloque con la dispersion que corresponde
            this->crear_bloque(tamDispersion*2, &bloqueNuevo);
            this->crear_bloque(tamDispersion*2, &bloqueAux);
        }
        cant_bloques++;
        //agrego los bloques vacios con el tamanio de dispersion que corresponde
        this->manejador_bloques.sobreescribir_bloque(&bloqueAux, posBloque);
        this->manejador_bloques.sobreescribir_bloque(&bloqueNuevo, posBloqueNuevo);
        //Agrego todos los registros del bloque que desbordo junto con el nuevo registro
        this->agregar_registros_bloques(bloque, reg);
        bloqueNuevo.~Bloque();
        bloqueAux.~Bloque();
        resultado= RES_OK;
    }
    this->manejador_bloques.cerrar_archivo();
    return resultado;
}

int HashingExtensible::devolver(ClaveX clave, RegistroClave *reg)
{
    int posReg;
    Bloque bloque;
    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //Obtengo el bloque que buscamos
    this->obtener_bloque(clave, &bloque);
    //Modifico el elemento del bloque
    posReg = this->obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)
    	return NO_EXISTE;
    bloque.recuperar_registro(reg, posReg);
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::modificar(RegistroClave elemN)
{
    Bloque bloque;
    int posReg, posBloque;
    ClaveX clave_reg =  elemN.get_clave();
    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //obtengo el bloque que buscamos
    posBloque = this->obtener_bloque(clave_reg, &bloque);
    //Busco el registro a modificar
    posReg = this->obtener_posicion_reg_bloque(clave_reg, bloque);
    if (posReg != RES_ERROR) bloque.eliminar_registro(posReg); //Borro el viejo registro
    bloque.agregar_registro(&elemN);    //Guardo el nuevo registro
    //Modifico el bloque en los bloques
    this->manejador_bloques.sobreescribir_bloque(&bloque, posBloque);
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::eliminar(ClaveX clave)
{
    int posBloque, tamDispersion, i, posDer, posIzq, posReg, posTabla;
    Bloque bloque;
    ClaveX valor;
    RegistroVariable tamDisp;

    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //Busco el bloque para agregar el elemento
    posTabla = this->obtener_posicion_tabla(clave);
    posBloque = this->obtener_bloque(clave, &bloque);
    posReg = this->obtener_posicion_reg_bloque(clave, bloque);
    if (posReg == RES_ERROR)
    	return RES_OK; // No esta la clave en el archivo, es lo mismo que borrarla
    //Elimino el registro
    bloque.eliminar_registro(posReg);
    if (bloque.get_cantidad_registros_almacenados() == 1){ // o sea que solo esta el tam dispersion
        //Busco el tamanio de dispersion del bloque
        bloque.recuperar_registro(&tamDisp, 0);
        char* buffer = new char[50]();
        tamDisp.recuperar_campo(buffer, 0);
        valor.desempaquetar(buffer, 5);     /**************como saco el tamanio con que se empaqueto?????****************/
        valor.get_clave(tamDispersion);

        //Me muevo tamDispersion para un lado y para el otro de la lista, si son iguales cambio cada tam disp *2 los bloques de la tabla
        posDer = this->tabla->obtener_valor(posTabla+tamDispersion);
        posIzq = this->tabla->obtener_valor(posTabla-tamDispersion);
        delete[] buffer;
        if (posDer == posIzq){
            //Eliminamos el bloque
            Bloque* bloqueVacio = this->manejador_bloques.crear_bloque();
            this->manejador_bloques.sobreescribir_bloque(bloqueVacio,posBloque);
            delete (bloqueVacio);
            this->cant_bloques --;
            //Reemplazamos en donde estaba la posicion del bloque viejo por la posicion que corresponda
 /**/           for(i=posTabla; i<tabla->get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                this->tabla->cambiar_valor(i,posDer);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                this->tabla->cambiar_valor(i,posDer);
            }
            this->dividir_tabla();
        }
    }
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::crear_bloque(int tam, Bloque *bloqueNuevo)
{
//Creamos el bloque con el tam de dispersion del tama�o de la tabla
    RegistroVariable tamDispBloque;

    //agregamos el tamanio al bloque
    std::stringstream ss;
    std::string campo;
    ss << tam;
    campo = ss.str();
    //Le guardo el tamaño de dispersion
    tamDispBloque.agregar_campo(campo.c_str(),campo.size());
    bloqueNuevo->agregar_registro(&tamDispBloque);
    return RES_OK;
}

int HashingExtensible::obtener_bloque(ClaveX clave, Bloque *bloque)
{
    int posBloque;
    //Busco el numero de bloque que necesitamos
    posBloque = this->tabla->obtener_valor(this->obtener_posicion_tabla(clave));
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
        clave_reg = reg.get_clave();
        if(clave_reg == clave)
        	se_encontro=RES_OK;
        i++;
    }
    if (se_encontro ==RES_OK)
    	return i;
    return RES_ERROR;
}

int HashingExtensible::obtener_posicion_tabla(ClaveX registro)
{   //Se devuelve la clave pasada por la funcion dispersion
    int clave=0;
    unsigned int i;
    if(registro.get_tipo_clave() == CLAVE_STRING){
        string claveS;
        registro.get_clave(claveS);
        for(i=0; i<claveS.length(); i++)   clave = clave+(int)claveS[i];
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
    int i=0, mitad=(this->tabla->get_tamanio())/2, resultado=RES_OK;
    while ((resultado == RES_OK)&&(i < mitad)){
        if((this->tabla->obtener_valor(i)) != (this->tabla->obtener_valor(i+mitad)))
            resultado = RES_ERROR;
        i++;
    }
    return RES_OK;
}
