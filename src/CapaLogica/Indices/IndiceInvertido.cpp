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
	RegistroVariable docInterseccion, terminos_frase;
	//Busco la interseccion de las lista de los documentos de cada termino de la frase
	if(this->interseccion_listas_invertidas(frase, docInterseccion) != RES_OK) return NO_EXISTE;
	//Armo un archivo termporal para buscar las canciones que tienen la frase
	if(this->armar_archivo_terminos_frase(frase, docInterseccion, terminos_frase) != RES_OK)  return RES_ERROR;
	//Armo lista que contienen la frase dentro de la cancion
	return this->buscar_doc_con_frase(terminos_frase);
}

int IndiceInvertido::interseccion_listas_invertidas(std::string frase, RegistroVariable &canciones)
{
    /***aca buscamos la interseccion de estas listas por IDdoc y guardamos las listas en un nuevo archivo de reg variables con el nombre pasado por parametro**/
	Texto texto;
	int resultado = RES_OK;
	RegistroClave regTerminoVoc;
	RegistroVariable regTermino, docInterseccion, listaCanTer, listaCancionesAux;
	ClaveX clave;
	std::string termino;
	texto.parsear(frase);
	//Saco la primera palabra de la frase
	if (texto.get_proxima_palabra(termino) == RES_ERROR) return NO_EXISTE;
	//Busco las canciones que tienen este termino en la letra
	resultado = this->obtener_canciones_termino(termino.c_str(), canciones);
	if(resultado != RES_OK)	return resultado;
	while(texto.get_proxima_palabra(termino) != RES_ERROR){
		//Busco las canciones que tienen este termino en la letra
		resultado = this->obtener_canciones_termino(termino.c_str(), listaCancionesAux);
		if(resultado != RES_OK)	return resultado;
		//Veo que canciones se encuentran en ambas listas
		if (this->interseccion(canciones, listaCancionesAux) != RES_OK) return NO_EXISTE;
	}
	return RES_OK;
}

int IndiceInvertido::interseccion(RegistroVariable &canciones, RegistroVariable &listaAux)
{	//Busco la interseccion de las dos listas y la guardo en la lista canciones
	int IDcan, IDcanAux, i=1, j=1, k=0;
	RegistroVariable interseccion;
	//Obtenemos los primeros IDcanciones
	canciones.recuperar_campo((char*)&IDcan, 0);
	listaAux.recuperar_campo((char*)&IDcanAux, 0);
	while((i<canciones.get_cantidad_campos())&&(j<listaAux.get_cantidad_campos())){
		if(IDcan<IDcanAux){
			canciones.recuperar_campo((char*)&IDcan, i);
			i++;
		}else{
			if(IDcan>IDcanAux){
				listaAux.recuperar_campo((char*)&IDcan, i);
				j++;
			}else{
				interseccion.agregar_campo((char*)&IDcan, k);
				k++;
			}
		}
	}
	//Actualizo las canciones
	canciones.limpiar_campos();
	for(i=0; i<k; i++){
		interseccion.recuperar_campo((char*)&IDcan, i);
		canciones.agregar_campo((char*)&IDcan, i);
	}
	return RES_OK;
}

int IndiceInvertido::obtener_canciones_termino(const char *termino, RegistroVariable &canciones)
{//Guarda en la lista de canciones todas aquellas canciones en las que aparece el termino pasado por parametro
	unsigned short ref_lista, i, cant_listas;
	int IDcan;
	RegistroClave regTerminoVoc, listaCan;
	RegistroVariable regTermino, listaCanTer;
	char *listaEmpaquetada;
	ClaveX clave, clave_cancion;
	clave.set_clave(termino);
	//Busco el termino en el vocabulario
	if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR)	return NO_EXISTE;
	//Recupero la lista de canciones que contienen ese termino
	regTerminoVoc.recuperar_campo((char*)&ref_lista, 2);
	this->listas_invertidas.devolver(&listaCanTer, ref_lista);
	cant_listas= listaCanTer.get_cantidad_campos();
	for(i=0; i<cant_listas; i++){
		//Obtengo cancion que contiene el termino pasado por parametro
		listaCanTer.recuperar_campo(listaEmpaquetada, i);
		listaCan.desempaquetar(listaEmpaquetada);
		clave_cancion = listaCan.get_clave();
		clave_cancion.get_clave(IDcan);
		//Guardo el IDcancion en la lista de canciones
		canciones.agregar_campo((char*)&IDcan, i);
	}
	return RES_OK;
}

int IndiceInvertido::armar_archivo_terminos_frase(std::string frase, RegistroVariable canciones, RegistroVariable &terminos_frase)
{
	Texto texto;
	int i, cant_canciones, IDcan, cant_pos, t=0;
	unsigned short IDter, ref_lista, ref_lista_pos, j;
	RegistroClave regTerminoVoc, listaCan;
	RegistroVariable regTermino, listaCanTer, listaPos;
	ClaveX clave, clave_cancion;
	char* listaEmpaquetada, *pos;
	std::string termino;
	ManejadorRegistrosVariables archivo_temp;
	if (archivo_temp.crear_archivo(this->ruta+"archivo_terminos_canciones") != RES_OK) return RES_ERROR;
	archivo_temp.abrir_archivo(this->ruta+"archivo_terminos_canciones");
	texto.parsear(frase);
	while(texto.get_proxima_palabra(termino) != RES_ERROR){
		//Obtengo el IDter
		this->vocabulario.buscar(regTerminoVoc);
		regTerminoVoc.recuperar_campo((char*)&IDter, 1);
		//La agrego a terminos_frase
		terminos_frase.agregar_datos((char*)&IDter, t);
		t++;
		//Obtengo la lista de canciones del termino
		regTerminoVoc.recuperar_campo((char*)&ref_lista, 2);
		this->listas_invertidas.devolver(&listaCanTer, ref_lista);
		cant_canciones = listaCanTer.get_cantidad_campos();
		//Armo un archivo termporal para buscar las canciones que tienen la frase
		for(i=0; i<cant_canciones; i++){
			//Obtengo cancion que contiene la lista
			listaCanTer.recuperar_campo(listaEmpaquetada, i);
			listaCan.desempaquetar(listaEmpaquetada);
			//Recupero el IDcan
			clave_cancion = listaCan.get_clave();
			clave_cancion.get_clave(IDcan);
			if(this->buscar_cancion_en_lista(IDcan, canciones) == RES_OK){
				//La cancion se encuentra entre las canciones que buscamos
				//Buscamo la lista de posiciones
				listaCan.recuperar_campo((char*) &ref_lista_pos, 2);
				this->listas_posiciones.devolver(&listaPos, ref_lista_pos);
				cant_pos = listaPos.get_cantidad_campos();
				for(j=0; j<cant_pos; j++){
					//Armo el registro (IDcancion, Pos, IDtermino)
					RegistroVariable regTem;
					regTem.agregar_campo((char*)&IDcan,0);
					listaPos.recuperar_campo(pos, j);
					regTem.agregar_campo(pos, 1);
					regTem.agregar_campo((char*)&IDter,2);
					//Agrego el registro al archivo
					archivo_temp.agregar_registro(&regTem);
				}
			}
		}
	}
	return RES_OK;
}

int IndiceInvertido::buscar_cancion_en_lista(int IDcancion, RegistroVariable &canciones)
{	//Busca dentro de la la lista de canciones si se encuenta la cancion pasada por parametro
	int i, IDaux, cant = canciones.get_cantidad_campos();
	for(i=0; i<cant; i++){
		canciones.recuperar_campo((char*)&IDaux, i);
		if(IDaux == IDcancion)	return RES_OK;
	}
	return RES_ERROR;
}

int IndiceInvertido::buscar_doc_con_frase(RegistroVariable terminos_frase)
{
	/*************/
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


int IndiceInvertido::buscar(char *elem_busqueda, std::string conjunto_iddoc)
{
    //Lo van a implementar cada indice invertido
    return RES_OK;
}

void IndiceInvertido::crear_reg_lista_inverida(RegistroVariable *regInvertido, char *pos, int IDdoc)
{
    //Lo van a implementar cada indice invertido
}
