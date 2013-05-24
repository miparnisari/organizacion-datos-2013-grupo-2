#include "IndiceInvertido.h"

/******falta implementar borrar_documento(int IDdoc)**/

IndiceInvertido::IndiceInvertido()
{

}

IndiceInvertido::~IndiceInvertido()
{

}

int IndiceInvertido::crear_indice(std::string directorioSalida, std::string fileNamee)
{
    this->fileName= directorioSalida+fileNamee;
    this->archivo_terminos.crear_archivo(this->fileName+"Terminos");
    this->vocabulario.crear(this->fileName+"Vocabulario", BLOQUE_TAM_DEFAULT);
    this->listas_invertidas.crear(directorioSalida, "ListasInvertidas");
    this->listas_posiciones.crear(directorioSalida, "ListasPosiciones");
    return RES_OK;
}

int IndiceInvertido::abrir_indice(std::string directorioSalida, std::string fileNamee)
{
    int resultado=0;
    this->fileName= directorioSalida+fileNamee;
    this->ruta=directorioSalida;
    resultado = resultado + this->archivo_terminos.abrir_archivo(this->fileName+"Terminos");
    resultado = resultado + this->vocabulario.abrir(this->fileName+"Vocabulario", "rb+");
    resultado = resultado + this->listas_invertidas.abrir(directorioSalida, "ListasInvertidas");
    resultado = resultado + this->listas_posiciones.abrir(directorioSalida, "ListasPosiciones");
    if (resultado != 0) return NO_EXISTE_INDICE;
    return RES_OK;
}

int IndiceInvertido::agregar_cancion(RegistroCancion cancion, int IDcancion)
{
    //Guardamos los terminos y armos las listas invertidas
	std::string letra = cancion.get_letra();
	//Creo el archivo de coincidencias
    if(this->archivo_coincidencias.crear_archivo(this->fileName+"Coincidencias") == RES_ERROR) return RES_ERROR;
    if(this->archivo_coincidencias.abrir_archivo(this->fileName+"Coincidencias") == RES_ERROR) return RES_ERROR;
	//Verifica si se encuentra en el vocabulario y se crea el archivo de coincidencias
    this->armar_archivo_coincidencias(letra, IDcancion);
    this->armar_listas_invertidas(IDcancion);
    //Borror el archivo de coincidencias
    this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias");
    return RES_OK;
}

int IndiceInvertido::armar_archivo_coincidencias(std::string letra, int IDcancion)
{
	Texto texto;
	int pos=0;
	long IDter, ref_lista;
	RegistroClave regTerminoVoc;
	RegistroVariable regTermino, regCoincidencia;
	ClaveX clave;
	std::string termino;
	texto.parsear(letra);
	while(texto.get_proxima_palabra(termino) != RES_ERROR){
		clave.set_clave(termino);
		if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR){
			//Como no existe en el vocabulario entonces creamos un nuevo registro y lo guardamos en el vocabulario
			RegistroVariable listaInvertida;
			//Agrego el termino al archivo de terminos
			regTermino.agregar_campo(termino.c_str(),0);
			IDter = this->archivo_terminos.agregar_registro(&regTermino);
			if(IDter == RES_ERROR)	return RES_ERROR;
			//Le agrego al reg del vocabulario el IDter
			regTerminoVoc.agregar_campo((char *)&IDter,1);
			//Le agrego al reg del vocabulario una referencia a una lista nueva
			ref_lista = this->listas_invertidas.agregar(&listaInvertida);
			regTerminoVoc.agregar_campo((char *)&ref_lista, 2);
			//Agrego el registro al vocabulario
			if(this->vocabulario.agregar(regTerminoVoc) != RES_OK)	return RES_ERROR;
		}else{
			//Como ya existe el termino entonces solo busco el IDtermino
			regTerminoVoc.recuperar_campo((char *)&IDter,1);
		}
		//Agrego el registro de coincidencia que corresponde
		regCoincidencia.agregar_campo((char *)&IDter,0);
		regCoincidencia.agregar_campo((char *)&pos,1);
		this->archivo_coincidencias.agregar_registro(&regCoincidencia);
		pos++;
	}
	/**Ordeno el archivo por IDter+pos*/
	return RES_OK;
}

int IndiceInvertido::armar_listas_invertidas(int IDcancion)
{
	RegistroVariable regCoincidencia, regTermino, listaInvertida;
	RegistroClave regTerminoVoc, regCancionTermino;
	ClaveX clave, claveTermino;
	unsigned short i=0, j;
	long IDter, IDterAnterior, ref_lista, ref_lista_pos;
	char* termino, *reg_cancion_termino, *pos;

	while (i<this->archivo_coincidencias.get_cantidad_registros_ocupados()){
		RegistroVariable listaPos;
		//Saco el registro de coincidencia
		this->archivo_coincidencias.get_registro_ocupado(&regCoincidencia, i);
		//Obtengo el IDter
		regCoincidencia.recuperar_campo((char *)&IDter, 0);
		IDterAnterior = IDter;
		//Busco el termino con ese IDter
		this->archivo_terminos.get_registro_por_offset(&regTermino, IDter);
		regTermino.recuperar_campo(termino, 0);
		//Busco la referencia de las lista lista invertida de este termino
		claveTermino.set_clave(termino);
		regTerminoVoc.set_clave(claveTermino);
		if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR) return RES_ERROR;
		regTerminoVoc.recuperar_campo((char*)&ref_lista,2);
		//Obtengo la lista invertida del termino
		this->listas_invertidas.devolver(&listaInvertida, ref_lista);
		//Armo la lista de posiciones
		j=0;
		do{//Mientras sean el mismo termino
			//Guardo la posicion en la lista de posiciones
			regCoincidencia.recuperar_campo(pos,1);
			listaPos.agregar_campo(pos, j);
			//Guardo el IDter como el anterior
			IDterAnterior = IDter;
			//Saco el proximo registro de coincidencia
			this->archivo_coincidencias.get_registro_ocupado(&regCoincidencia, i);
			//Obtengo el IDter
			regCoincidencia.recuperar_campo((char *)&IDter, 0);
			i++;
			j++;
		}while(IDterAnterior == IDter);
		//Guardo la lista de posiciones para el termino en esa cancion y obtengo su referencia
		ref_lista_pos = this->listas_posiciones.agregar(&listaPos);
		//Creo la lista de posiciones de la cancion IDcancion
		clave.set_clave(IDcancion);
		regCancionTermino.set_clave(clave);
		//Le agrego la referencia a la lista de canciones por termino
		regCancionTermino.agregar_campo((char *)&ref_lista_pos, 1);
		//Guardo esta lista en la lista invertida de canciones
		regCancionTermino.empaquetar(reg_cancion_termino);
		listaInvertida.agregar_campo(reg_cancion_termino, 0);
		this->listas_invertidas.recontruir_listas(ref_lista, listaInvertida);
	}
	return RES_OK;
}

int IndiceInvertido::buscar_frase(std::string frase, RegistroVariable &lista)
{
	Texto texto;
	/*int pos=0;
	long IDter, ref_lista;*/
	RegistroClave regTerminoVoc;
	RegistroVariable regTermino, regCoincidencia;
	ClaveX clave;
	std::string termino;
	texto.parsear(frase);
	while(texto.get_proxima_palabra(termino) != RES_ERROR){
		clave.set_clave(termino);
		if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR)	return NO_EXISTE;

	}
	return RES_OK;
}


int IndiceInvertido::borrar_indice()
{
    this->archivo_terminos.eliminar_archivo(this->fileName+"Terminos");
    this->vocabulario.eliminar(this->fileName+"Vocabulario");
    this->listas_invertidas.eliminar(this->ruta, "ListasInvertidas");
    this->listas_posiciones.eliminar(this->ruta, "ListasPosiciones");
    return RES_OK;
}

/*
int IndiceInvertido::buscar_lista(int IDterLista, RegistroVariable &lista)
{
    //Devolvemos la posicion de la lista dentro del archivo de listas invertidas y la lista que corresponde
    RegistroClave reg;
    ClaveX clave;
    char *termino, *refLista, *idtermino; //= itoa (IDterLista, idtermino, 10); *****chanchada para guardar numero como char***/
//    int num;
    /*****busco el termino en el archivo de terminos, busqueda binaria*/
/*    clave.set_clave(termino);
    reg.set_clave(clave);
*********    vocabulario.buscar(reg);**********/
/*    reg.recuperar_campo(refLista,2);
    num = atoi(refLista);   //recupero el numero
    if(num<0){
        //Modifico el registro para que guarde como referencia a la lista el primer bloque libre
        num = listas_invertidas.get_primer_bloque_libre();
        reg.limpiar_campos();
        reg.set_clave(clave);   //Guardo el termino
        reg.agregar_campo(idtermino, sizeof(idtermino));  //Guardo el IDtermino
        reg.agregar_campo(refLista, sizeof(refLista));  //Guardo la referencia a la lista
        lista = listas_invertidas.crear_bloque();
    }else{
        lista = listas_invertidas.obtener_bloque(num);
    }
    return num;
}
*/
int IndiceInvertido::interseccion_listas_invertidas(std::string archivo)
{
    /***aca buscamos la interseccion de estas listas por IDdoc y guardamos las listas en un nuevo archivo de reg variables con el nombre pasado por parametro**/
    return RES_OK;
}


int IndiceInvertido::buscar(char *elem_busqueda, std::string conjunto_iddoc)
{
    //Lo van a implementar cada indice invertido
    return RES_OK;
}

void IndiceInvertido::crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc)
{
    //Lo van a implementar cada indice invertido
}
