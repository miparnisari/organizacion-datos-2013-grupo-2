#include "../HashingExtensible/HashingExtensible.h"
#include <sstream>

/**No se necesita guardar nada del Hash porque todo se puede recuperar con el constructor

Falta revisar como se guardan los tamanios de dispersion de los bloques*/

HashingExtensible::HashingExtensible()
{
	this->fileName = "";
	this->fileNameTabla = "";
	this->cant_bloques = 0;
    this->tabla = new Tabla();
}

HashingExtensible::~HashingExtensible()
{
    delete(tabla);
}

int HashingExtensible::abrir_archivo(std::string nombreArchivo)
{
	int resultado;
	this->fileName=nombreArchivo+".dat";
    this->fileNameTabla=nombreArchivo+"Tabla"+".dat";
	resultado = this->manejador_bloques.abrir_archivo(fileName, "rb+");
    this->cant_bloques = manejador_bloques.get_cantidad_bloques();

//    this->manejador_bloques.cerrar_archivo();
//    resultado = resultado + this->tabla->crear(fileNameTabla);


    this->tabla->set_ruta(this->fileNameTabla);
    return resultado;
}

int HashingExtensible::crear_archivo(std::string nombreArchivo)
{
    this->fileName=nombreArchivo+".dat";
    this->fileNameTabla=nombreArchivo+"Tabla.dat";

    if (this->manejador_bloques.abrir_archivo(fileName, "rb+") == RES_OK)
    {
    	this->manejador_bloques.cerrar_archivo();
    	return YA_EXISTE;
    }

    //Si no existe es porque estamos creando un nuevo Hash
    int resultado = this->manejador_bloques.crear_archivo(fileName, 1024);//tamanio anterior TAM_DEFAULT
	this->manejador_bloques.abrir_archivo(fileName, "rb+");
	//Creamos la tabla
	resultado = resultado + this->tabla->crear(fileNameTabla);
	this->tabla->cambiar_valor(0, 0); // la pos 0 de la tabla se le coloca el num de bloque 0


	//Creamos un nuevo bloque con dispersion 1 y lo guardamos en el archivo de bloques del Hash
	Bloque* bloqueNuevo = this->manejador_bloques.crear_bloque();
	this->_crear_bloque(1, bloqueNuevo);
	int res = this->manejador_bloques.escribir_bloque(bloqueNuevo);//Va a estar en la posicion 0
	if (res < 0)
	{
		return RES_ERROR;
	}
	delete (bloqueNuevo);
	this->cant_bloques = 1;
    this->manejador_bloques.cerrar_archivo();
    return resultado;
}

int HashingExtensible::eliminar_archivo()
{
	int resultado1 = this->manejador_bloques.eliminar_archivo(this->fileName);
	int resultado2 = tabla->eliminar();
	if (resultado1 == RES_OK && resultado2 == RES_OK)
			return RES_OK;
	return RES_ERROR;
}

int HashingExtensible::_funcion_dispersion(int clave)
{
    int tam_tabla = this->tabla->get_tamanio();
    return (clave % tam_tabla);
}

int HashingExtensible::agregar(RegistroClave & reg)
{
    int posBloque, i, resultado, posBloqueNuevo, posReg, tamTabla, posTabla;
    Bloque* bloque = NULL;
    unsigned int tamDispersion;
    RegistroVariable tamDispBloque;
    ClaveX clave_reg = reg.get_clave();

    tamTabla = tabla->get_tamanio();
    //Obtengo el bloque en donde se guardara el registro

    posBloque = this->_obtener_bloque(clave_reg, &bloque);
    posTabla = this->_obtener_posicion_tabla(clave_reg);

    //Verificamos que no exista la clave del registro que queremos guardar
    posReg = this->_obtener_posicion_reg_bloque(clave_reg, *bloque);
    if (posReg != RES_ERROR) {
    	//this->manejador_bloques.cerrar_archivo();
    	delete bloque;
    	return YA_EXISTE;
    }

    //Agrego el elemento
    resultado = bloque->agregar_registro(&reg);

    //Si desborda el bloque
    if (resultado == RES_INSUFFICIENT_SPACE) {
        Bloque* bloqueNuevo = this->manejador_bloques.crear_bloque();
        Bloque* bloqueAux = this->manejador_bloques.crear_bloque();
        //Recupero el tamaño de dispersion del bloque
        bloque->recuperar_registro(&tamDispBloque,0);
        tamDispBloque.recuperar_campo((char*)&tamDispersion,0);

        //La pos del nuevo bloque es el libre
        posBloqueNuevo = this->manejador_bloques.escribir_bloque(bloqueNuevo);

        if(tamDispersion == (unsigned)tamTabla){

            //Duplicamos la tabla
        	this->tabla->duplicar();
        	this->tabla->cambiar_valor(posTabla, posBloqueNuevo);

            //Creamos el bloque con el tam de dispersion del tamanio de la tabla
            this->_crear_bloque(tamTabla*2, bloqueNuevo);
            this->_crear_bloque(tamTabla*2, bloqueAux);

        }else{

            //Modifico en la tabla la posicion por cantidad de tamDispersion
        	for(i=posTabla; i<tabla->get_tamanio(); i=i+tamDispersion*2){ //hacia la derecha
                this->tabla->cambiar_valor(i,posBloqueNuevo);
            }
            for(i=posTabla; i>=0; i=i-tamDispersion*2){ // hacia la izquierda
                this->tabla->cambiar_valor(i,posBloqueNuevo);
            }
            //Creo un bloque con la dispersion que corresponde
            this->_crear_bloque(tamDispersion*2, bloqueNuevo);
            this->_crear_bloque(tamDispersion*2, bloqueAux);
        }
        cant_bloques++;
        //agrego los bloques vacios con el tamanio de dispersion que corresponde
        this->manejador_bloques.sobreescribir_bloque(bloqueAux, posBloque);
        this->manejador_bloques.sobreescribir_bloque(bloqueNuevo, posBloqueNuevo);
        //Agrego todos los registros del bloque que desbordo junto con el nuevo registro
        this->_agregar_registros_bloques(*bloque, reg);

        resultado = RES_OK;
    }else{
    	this->manejador_bloques.sobreescribir_bloque(bloque, posBloque);
    }

    delete bloque;
    //this->manejador_bloques.cerrar_archivo();
    return resultado;
}

int HashingExtensible::cerrar_archivo(){
	return this->manejador_bloques.cerrar_archivo();
}


int HashingExtensible::devolver(ClaveX & clave, RegistroClave *reg)
{
	reg->limpiar_campos();
    int posReg;
    Bloque* bloque = NULL;
    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //Obtengo el bloque que buscamos
    int numBloque = this->_obtener_bloque(clave, &bloque);
    if (numBloque == RES_ERROR)
    {
    	delete bloque;
    	this->manejador_bloques.cerrar_archivo();
    	return NO_EXISTE;
    }
    //Modifico el elemento del bloque

    posReg = this->_obtener_posicion_reg_bloque(clave, *bloque);

    if (posReg == RES_ERROR)
    {
    	delete bloque;
    	this->manejador_bloques.cerrar_archivo();
        return NO_EXISTE;
    }

    bloque->recuperar_registro(reg, posReg);
    delete bloque;
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::modificar(RegistroClave & elemN)
{
    Bloque* bloque = NULL;
    int posReg, posBloque;
    ClaveX clave_reg =  elemN.get_clave();
    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //obtengo el bloque que buscamos
    posBloque = this->_obtener_bloque(clave_reg, &bloque);
    //Busco el registro a modificar
    posReg = this->_obtener_posicion_reg_bloque(clave_reg, *bloque);
    if (posReg != RES_ERROR) bloque->eliminar_registro(posReg); //Borro el viejo registro
    bloque->agregar_registro(&elemN);    //Guardo el nuevo registro
    //Modifico el bloque en los bloques
    this->manejador_bloques.sobreescribir_bloque(bloque, posBloque);
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::eliminar(ClaveX & clave)
{
    int posBloque, i, posDer, posIzq, posReg, posTabla;
    unsigned int tamDispersion;
    Bloque* bloque = NULL;
    RegistroVariable regTamDisp;

    this->manejador_bloques.abrir_archivo(fileName, "rb+");
    //Busco el bloque para agregar el elemento
    posTabla = this->_obtener_posicion_tabla(clave);
    posBloque = this->_obtener_bloque(clave, &bloque);
    posReg = this->_obtener_posicion_reg_bloque(clave, *bloque);
    if (posReg == RES_ERROR)
    {
    	delete bloque;
        return RES_OK; // No esta la clave en el archivo, es lo mismo que borrarla
    }
    //Elimino el registro
    bloque->eliminar_registro(posReg);
    if (bloque->get_cantidad_registros_almacenados() == 1){ // o sea que solo esta el tam dispersion
        //Busco el tamanio de dispersion del bloque
        bloque->recuperar_registro(&regTamDisp, 0);
        regTamDisp.recuperar_campo((char*)&tamDispersion,0);

        //Me muevo tamDispersion para un lado y para el otro de la lista, si son iguales cambio cada tam disp *2 los bloques de la tabla
        posDer = this->tabla->obtener_valor(posTabla+tamDispersion);
        posIzq = this->tabla->obtener_valor(posTabla-tamDispersion);
        // FIXME aca hay un error! no se esta chequeando que las dos cosas anteriores pueden ser -1!!
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
            this->_dividir_tabla();
        }
    }
    delete bloque;
    this->manejador_bloques.cerrar_archivo();
    return RES_OK;
}

int HashingExtensible::_crear_bloque(int tam, Bloque* bloqueNuevo)
{
        if (bloqueNuevo == NULL)
                return RES_ERROR;
    RegistroVariable regTamDisp;
    regTamDisp.agregar_campo((char*)&tam,sizeof(tam));
    bloqueNuevo->agregar_registro(&regTamDisp);
    return RES_OK;
}

int HashingExtensible::_obtener_bloque(ClaveX & clave, Bloque** bloque)
{
    int posBloque;
    //Busco el numero de bloque que necesitamos

    int posTabla = this->_obtener_posicion_tabla(clave);

    posBloque = this->tabla->obtener_valor(posTabla);
    if (posBloque < 0)
    	return RES_ERROR;

    Bloque* unbloque = this->manejador_bloques.obtener_bloque(posBloque);
    *bloque = unbloque;

    return posBloque;
}

int HashingExtensible::_obtener_posicion_reg_bloque(ClaveX & clave, Bloque& bloque)
{
    int i = 1,se_encontro = RES_ERROR;
    ClaveX clave_reg;
    RegistroClave reg;

    while((se_encontro!=RES_OK)&&(i< bloque.get_cantidad_registros_almacenados())){
        bloque.recuperar_registro(&reg, i);
        clave_reg = reg.get_clave();
        if(clave_reg== clave)
                se_encontro=RES_OK;
        i++;
    }
    if (se_encontro ==RES_OK)
        return (i -1);
    return RES_ERROR;
}

int HashingExtensible::_obtener_posicion_tabla(ClaveX&  registro)
{   //Se devuelve la clave pasada por la funcion dispersion
    int clave=0;
    unsigned int i;
    if(registro.get_tipo_clave() == CLAVE_STRING){
        string claveS;
        registro.get_clave(claveS);
        for(i=0; i<claveS.length(); i++){
        	clave = clave + (int)claveS[i];
        }
    }else{
        registro.get_clave(clave);
    }
    return _funcion_dispersion(clave);
}

int HashingExtensible::_agregar_registros_bloques(Bloque & bloque, RegistroClave & reg)
{   //Inserta todos los registros y en caso que se desborde el agregar ya se encarga de arreglarlo
    RegistroClave regAux;
    int i;
    int cant = bloque.get_cantidad_registros_almacenados();
    for(i=1; i<cant; i++){
        bloque.recuperar_registro(&regAux, i);
        this->agregar(regAux);
    }
    this->agregar(reg);
    return RES_OK;
}

int HashingExtensible::_dividir_tabla()
{
    int i=0;
    int mitad=(this->tabla->get_tamanio())/2;
    int resultado=RES_OK;
    while ((resultado == RES_OK)&&(i < mitad)){
        if((this->tabla->obtener_valor(i)) != (this->tabla->obtener_valor(i+mitad)))
            resultado = RES_ERROR;
        i++;
    }
    return RES_OK;
}
