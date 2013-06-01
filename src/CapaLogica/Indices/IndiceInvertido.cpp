#include "IndiceInvertido.h"

/******TODO implementar borrar_documento(int IDdoc)**/

IndiceInvertido::IndiceInvertido()
{

}

IndiceInvertido::~IndiceInvertido()
{

}

int IndiceInvertido::crear_indice(std::string directorioSalida, std::string fileNamee)
{
	this->fileName= directorioSalida+fileNamee;
	int res = this->archivo_terminos.crear_archivo(this->fileName+"Terminos.dat");
	res += this->vocabulario.crear(this->fileName+"Vocabulario.dat", BLOQUE_TAM_DEFAULT);
	res += this->listas_invertidas.crear(directorioSalida, "ListasInvertidas");
	res += this->listas_posiciones.crear(directorioSalida, "ListasPosiciones");
	return res;
}

int IndiceInvertido::abrir_indice(std::string directorioSalida, std::string fileNamee)
{
	int resultado=0;
	this->fileName= directorioSalida+fileNamee;
	this->ruta=directorioSalida;
	resultado += this->archivo_terminos.abrir_archivo(this->fileName+"Terminos.dat");
	resultado += this->vocabulario.abrir(this->fileName+"Vocabulario.dat", "rb+");
	resultado += this->listas_invertidas.abrir(directorioSalida, "ListasInvertidas");
	resultado += this->listas_posiciones.abrir(directorioSalida, "ListasPosiciones");
	if (resultado != RES_OK)
		return NO_EXISTE_INDICE;
	return RES_OK;
}

int IndiceInvertido::cerrar_indice()
{
	int res= this->vocabulario.cerrar();
	return res;
}

int IndiceInvertido::agregar_texto(std::string texto, int IDtexto)
{
    if(this->archivo_coincidencias.crear_archivo(this->fileName+"Coincidencias.dat") == RES_ERROR)
    	return RES_ERROR;
    if(this->archivo_coincidencias.abrir_archivo(this->fileName+"Coincidencias.dat") == RES_ERROR)
    {
    	this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias.dat");
    	return RES_ERROR;
    }
	//Verifica si se encuentra en el vocabulario y se crea el archivo de coincidencias
    if (this->_armar_archivo_coincidencias(texto) == RES_ERROR)
    {
    	this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias.dat");
    	return RES_ERROR;
    }
    if (this->_armar_listas_invertidas(IDtexto) == RES_ERROR)
    {
    	this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias.dat");
    	return RES_ERROR;
    }
    //Borror el archivo de coincidencias
    this->archivo_coincidencias.eliminar_archivo(this->fileName+"Coincidencias.dat");
    return RES_OK;
}

int IndiceInvertido::_armar_archivo_coincidencias(std::string unTexto)
{
	Texto texto;
	int pos=0, IDter, ref_lista;
	RegistroClave regTerminoVoc;
	RegistroVariable regTermino, regCoincidencia;
	ClaveX clave;
	std::string termino;
	texto.parsear(unTexto);

	while(texto.get_proxima_palabra(termino) != RES_FIN){
		regTerminoVoc.limpiar_campos();
		clave.set_clave(termino);
		regTerminoVoc.set_clave(clave);
		if(this->vocabulario.buscar(regTerminoVoc) == RES_RECORD_DOESNT_EXIST){
			//Como no existe en el vocabulario entonces creamos un nuevo registro y lo guardamos en el vocabulario
			RegistroVariable listaInvertida;
			//Agrego el termino al archivo de terminos
			regTermino.limpiar_campos();
			regTermino.agregar_campo(termino.c_str(),termino.size());
			IDter = this->archivo_terminos.agregar_registro(&regTermino);
			if (IDter == RES_ERROR)
				return RES_ERROR;

			//Le agrego al reg del vocabulario una referencia a una lista nueva
			ref_lista = this->listas_invertidas.agregar(&listaInvertida); // FIXME listaInvertida esta VACIO!
			regTerminoVoc.agregar_campo((char *)&IDter, sizeof(ref_lista));
			regTerminoVoc.agregar_campo((char *)&ref_lista, sizeof(ref_lista));

			if(this->vocabulario.agregar(regTerminoVoc) != RES_OK)
				return RES_ERROR;
		}else{
			regTerminoVoc.recuperar_campo((char*)&IDter,1);
		}

		regCoincidencia.limpiar_campos();
		regCoincidencia.agregar_campo((char *)&IDter,sizeof(IDter));
		regCoincidencia.agregar_campo((char *)&pos,sizeof(pos));
		this->archivo_coincidencias.agregar_registro(&regCoincidencia);
		pos++;
	}
	//TODO: Ordeno el archivo ¿DE COINCIDENCIAS? por IDter+pos

	return RES_OK;
}

int IndiceInvertido::_armar_listas_invertidas(int IDcancion)
{
	RegistroVariable regCoincidencia, regTermino, listaInvertida;
	RegistroClave regTerminoVoc, regCancionTermino;
	ClaveX clave, claveTermino;
	unsigned short i=0;
	int IDter, IDterAnterior, ref_lista, ref_lista_pos;

	unsigned int q = this->archivo_coincidencias.get_cantidad_registros_ocupados();
	while (i <q){
		RegistroVariable listaPos;
		//Saco el registro de coincidencia
		this->archivo_coincidencias.get_registro_ocupado(&regCoincidencia, i);
		//Obtengo el IDter
		regCoincidencia.recuperar_campo((char *)&IDter, 0);
		IDterAnterior = IDter;
		//Busco el termino con ese IDter
		this->archivo_terminos.get_registro_por_offset(&regTermino, IDter);
		char* termino = new char[regTermino.get_tamanio_campo(0) + 1]();
		termino[regTermino.get_tamanio_campo(0)] = '\0';
		regTermino.recuperar_campo(termino, 0);

		claveTermino.set_clave(std::string(termino));
		delete[] termino;

		regTerminoVoc.limpiar_campos();
		//Busco la referencia de la lista invertida de este termino
		regTerminoVoc.set_clave(claveTermino);
		if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR)
			return RES_ERROR;
		regTerminoVoc.recuperar_campo((char*)&ref_lista,1);
		//Obtengo la lista invertida del termino
		this->listas_invertidas.devolver(&listaInvertida, ref_lista);
		//Armo la lista de posiciones
		int j=0;
		do{//Mientras sean el mismo termino
			//Guardo la posicion en la lista de posiciones
			int pos;
			regCoincidencia.recuperar_campo((char*)&pos,1);
			listaPos.agregar_campo((char*)&pos, sizeof(pos));
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
		regCancionTermino.agregar_campo((char *)&ref_lista_pos, sizeof(ref_lista_pos));
		//Guardo esta lista en la lista invertida de canciones
		char* reg_cancion_termino = new char[regCancionTermino.get_tamanio_empaquetado() + 1]();
		reg_cancion_termino[regCancionTermino.get_tamanio_empaquetado()] = '\0';
		regCancionTermino.empaquetar(reg_cancion_termino);
		listaInvertida.agregar_campo(reg_cancion_termino, strlen(reg_cancion_termino));
		delete[] reg_cancion_termino;
		this->listas_invertidas.recontruir_listas(ref_lista, listaInvertida);
	}
	return RES_OK;
}

int IndiceInvertido::buscar_frase(std::string frase, RegistroVariable & lista)
{
	RegistroVariable docInterseccion, terminos_frase;
	//Busco la interseccion de las lista de los documentos de cada termino de la frase
	if(this->_interseccion_listas_invertidas(frase, docInterseccion) != RES_OK)
		return NO_EXISTE;
	//Armo un archivo termporal para buscar las canciones que tienen la frase
	if(this->_armar_archivo_terminos_frase(frase, docInterseccion, terminos_frase) != RES_OK)
		return RES_ERROR;
	//Armo lista que contienen la frase dentro de la cancion
	return this->_buscar_cancion_con_frase(terminos_frase, lista);
}

int IndiceInvertido::_interseccion_listas_invertidas(std::string & frase, RegistroVariable & canciones)
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
	if (texto.get_proxima_palabra(termino) == RES_FIN)
		return NO_EXISTE;
	//Busco las canciones que tienen este termino en la letra
	resultado = this->_obtener_canciones_termino(termino.c_str(), canciones);
	if(resultado != RES_OK)
		return resultado;
	while(texto.get_proxima_palabra(termino) != RES_FIN){
		//Busco las canciones que tienen este termino en la letra
		resultado = this->_obtener_canciones_termino(termino.c_str(), listaCancionesAux);
		if(resultado != RES_OK)
			return resultado;
		//Veo que canciones se encuentran en ambas listas
		if (this->_interseccion(canciones, listaCancionesAux) != RES_OK)
			return NO_EXISTE;
	}
	return RES_OK;
}

int IndiceInvertido::_interseccion(RegistroVariable &canciones, RegistroVariable &listaAux)
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
				interseccion.agregar_campo((char*)&IDcan, sizeof(IDcan));
				k++;
			}
		}
	}
	//Actualizo las canciones
	canciones.limpiar_campos();
	for(i=0; i<k; i++){
		interseccion.recuperar_campo((char*)&IDcan, i);
		canciones.agregar_campo((char*)&IDcan, sizeof(IDcan));
	}
	return RES_OK;
}

int IndiceInvertido::_obtener_canciones_termino(const char *termino, RegistroVariable &canciones)
{//Guarda en la lista de canciones todas aquellas canciones en las que aparece el termino pasado por parametro
	unsigned short ref_lista, i, cant_listas;
	int IDcan;
	RegistroClave regTerminoVoc, listaCan;
	RegistroVariable regTermino, listaCanTer;

	ClaveX clave, clave_cancion;
	clave.set_clave(termino);
	//Busco el termino en el vocabulario
	if(this->vocabulario.buscar(regTerminoVoc) == RES_ERROR)
		return NO_EXISTE;
	//Recupero la lista de canciones que contienen ese termino
	regTerminoVoc.recuperar_campo((char*)&ref_lista, 1);
	this->listas_invertidas.devolver(&listaCanTer, ref_lista);
	cant_listas= listaCanTer.get_cantidad_campos();
	for(i=0; i<cant_listas; i++){
		//Obtengo cancion que contiene el termino pasado por parametro
		char* listaEmpaquetada = new char[listaCanTer.get_tamanio_campo(i)]();
		listaCanTer.recuperar_campo(listaEmpaquetada, i);
		listaCan.desempaquetar(listaEmpaquetada);
		clave_cancion = listaCan.get_clave();
		clave_cancion.get_clave(IDcan);
		//Guardo el IDcancion en la lista de canciones
		canciones.agregar_campo((char*)&IDcan, sizeof(IDcan));
		delete[] listaEmpaquetada;
	}
	return RES_OK;
}

int IndiceInvertido::_armar_archivo_terminos_frase(std::string & frase, RegistroVariable & canciones, RegistroVariable & terminos_frase)
{
	Texto texto;
	int i, cant_canciones, IDcan, cant_pos, t=0;
	int IDter;
	unsigned short ref_lista, ref_lista_pos, j;
	RegistroClave regTerminoVoc, listaCan;
	RegistroVariable regTermino, listaCanTer, listaPos;
	ClaveX clave, clave_cancion;
	std::string termino;
	ManejadorRegistrosVariables archivo_temp;
	if (archivo_temp.crear_archivo(this->ruta+"archivo_terminos_canciones") != RES_OK)
		return RES_ERROR;
	archivo_temp.abrir_archivo(this->ruta+"archivo_terminos_canciones");
	texto.parsear(frase);
	while(texto.get_proxima_palabra(termino) != RES_FIN){
		//Obtengo el IDter
		this->vocabulario.buscar(regTerminoVoc);
		ClaveX claveIDter = regTerminoVoc.get_clave();
		claveIDter.get_clave(IDter);
		//La agrego a terminos_frase
		terminos_frase.agregar_datos((char*)&IDter, sizeof(IDter));
		t++;
		//Obtengo la lista de canciones del termino
		regTerminoVoc.recuperar_campo((char*)&ref_lista, 1);
		this->listas_invertidas.devolver(&listaCanTer, ref_lista);
		cant_canciones = listaCanTer.get_cantidad_campos();
		//Armo un archivo termporal para buscar las canciones que tienen la frase
		for(i=0; i<cant_canciones; i++){
			//Obtengo cancion que contiene la lista
			char* listaEmpaquetada = new char[listaCanTer.get_tamanio_campo(i)]();
			listaCanTer.recuperar_campo(listaEmpaquetada, i);
			listaCan.desempaquetar(listaEmpaquetada);
			delete[] listaEmpaquetada;
			//Recupero el IDcan
			clave_cancion = listaCan.get_clave();
			clave_cancion.get_clave(IDcan);
			if(this->_buscar_cancion_en_lista(IDcan, canciones) == RES_OK){
				//La cancion se encuentra entre las canciones que buscamos
				//Buscamo la lista de posiciones
				listaCan.recuperar_campo((char*) &ref_lista_pos, 2); // FIXME 2 o 1?
				this->listas_posiciones.devolver(&listaPos, ref_lista_pos);
				cant_pos = listaPos.get_cantidad_campos();
				for(j=0; j<cant_pos; j++){
					//Armo el registro (IDcancion, Pos, IDtermino)
					RegistroVariable regTem;
					regTem.agregar_campo((char*)&IDcan,sizeof(IDcan));
					char* pos = new char[listaPos.get_tamanio_campo(j) +1]();
					pos[listaPos.get_tamanio_campo(j)] = '\0';
					listaPos.recuperar_campo(pos, j);
					regTem.agregar_campo(pos, strlen(pos));
					regTem.agregar_campo((char*)&IDter,sizeof(IDter));
					delete[] pos;
					//Agrego el registro al archivo
					archivo_temp.agregar_registro(&regTem);
				}
			}
		}
	}
	return RES_OK;
}

int IndiceInvertido::_buscar_cancion_en_lista(int IDcancion, RegistroVariable &canciones)
{	//Busca dentro de la la lista de canciones si se encuenta la cancion pasada por parametro
	int i, IDaux, cant = canciones.get_cantidad_campos();
	for(i=0; i<cant; i++){
		canciones.recuperar_campo((char*)&IDaux, i);
		if(IDaux == IDcancion)
			return RES_OK;
	}
	return RES_ERROR;
}

int IndiceInvertido::_buscar_cancion_con_frase(RegistroVariable & terminos_frase, RegistroVariable &lista)
{
	ManejadorRegistrosVariables archivo_temp;
	RegistroVariable reg;
	int IDcancion= -1, i=0, cant_reg, pos=-1, k=0;
	if (archivo_temp.abrir_archivo(this->ruta+"archivo_terminos_canciones") != RES_OK)
		return RES_ERROR;
	/****Ordeno el archivo**************************************************************************************/
	if (archivo_temp.abrir_archivo(this->ruta+"archivo_terminos_canciones") != RES_OK)
		return RES_ERROR;
	cant_reg = archivo_temp.get_cantidad_registros_ocupados();
	while(i<cant_reg){
		//Recorremos todos los reg del archivo temporal
		archivo_temp.get_registro_ocupado(&reg, i);
		//Obtengo su IDcancion
		reg.recuperar_campo((char*)&IDcancion, 0);
		//Obtengo su Pos
		reg.recuperar_campo((char*)&pos, 1);
		if(this->_siguiente_termino_frase(i, 0, pos, IDcancion, terminos_frase) != NO_PERTENECE){
			//Como IDcancion contiene la frase entonces se guarda este ID en la lista
			lista.agregar_campo((char*)&IDcancion, sizeof(IDcancion));
			k++;
		}
	}

	archivo_temp.eliminar_archivo(this->ruta+"archivo_terminos_canciones");
	return RES_OK;
}

int IndiceInvertido::_siguiente_termino_frase(int &pos_reg, int pos_ter_frase, int &posAnterior, int &IDcancionAnterior, RegistroVariable &terminos_frase)
{//Recursiva
	ManejadorRegistrosVariables archivo_temp;
	int IDter, IDterminoFrase, pos, IDcancion;
	RegistroVariable reg;
	if (archivo_temp.abrir_archivo(this->ruta+"archivo_terminos_canciones") != RES_OK)
		return RES_ERROR;
	//Saco el IDter de la frase que esta en la pos_ter_frase
	terminos_frase.recuperar_campo((char*)&IDterminoFrase, pos_ter_frase);
	pos_ter_frase++;
	//Saco el termino del archivo
	archivo_temp.get_registro_ocupado(&reg, pos_reg);
	pos_reg++;
	//Obtengo el IDcancion del registro
	reg.recuperar_campo((char*)&IDcancion,0);
	if(IDcancion == IDcancionAnterior){
		//Seguimos en la misma cancion
		//Recupero el IDter y la pos
		reg.recuperar_campo((char*)&IDter, 2);
		reg.recuperar_campo((char*)&pos, 1);
		if((IDter == IDterminoFrase)&&(pos-posAnterior<2)){
			//El termino esta en el orden que corresponde, asi que buscamos en el siguiente termino
			if(pos_reg == archivo_temp.get_cantidad_registros_ocupados()){
				//Entonces completamos la frase
				return RES_OK;
			}else{
				//Todavia hay terminos por ver
				return this->_siguiente_termino_frase(pos_reg, pos_ter_frase, pos, IDcancion, terminos_frase);
			}
		}else{
			//Entonces este documento no pertenece a la solucion
			return	NO_PERTENECE;
		}
	}
	return NO_PERTENECE;
}


int IndiceInvertido::borrar_indice()
{
	this->archivo_terminos.eliminar_archivo(this->fileName+"Terminos.dat");
	this->vocabulario.eliminar(this->fileName+"Vocabulario.dat");
	this->listas_invertidas.eliminar(this->ruta, "ListasInvertidas");
	this->listas_posiciones.eliminar(this->ruta, "ListasPosiciones");
	return RES_OK;
}
