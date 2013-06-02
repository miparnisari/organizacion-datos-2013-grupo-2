/*
 * SortExterno.cpp
 *
 *  Created on: May 16, 2013
 *      Author: juan
 */

#include "SortExterno.h"

SortExterno::SortExterno(string nombreArchivo) {
	cantDeRuns=0;
	archAOrdenar=nombreArchivo;
}

SortExterno::~SortExterno() {

}

void SortExterno:: _generar_runs()
{
	ManejadorRegistrosVariables archivoAOrdenar;

	archivoAOrdenar.abrir_archivo(archAOrdenar);

	string nombreGeneralDeRun="run";
	string extension=".dat";
	int cantRunsArmados=0;

	int cantRegLeidos=0;
	unsigned tamanioTotal=0;//del vector a ordenar por heapsort en RAM

	RegistroVariable regVariableLeido;
//	RegistroClave* bufferOrdenamiento;
	RegistroVariable bufferOrdenamiento[1000];//fixme

	//lleno el heap por primera vez
	//puede que resulta un poco mayor al limite (lo ultimo que leo se puede pasar)
	while ((tamanioTotal < TAM_MAX_BUFFER_ORDENAMIENTO)&&(archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos))
	{
		archivoAOrdenar.get_registro_ocupado(&regVariableLeido, cantRegLeidos);

//		char* campo=new char[regVariableLeido.get_tamanio_campo(0)+1]; //todo borrar
//		regVariableLeido.recuperar_campo(campo,0);
//		campo[regVariableLeido.get_tamanio_campo(0)]='\0';
//		string campoImprimir1 = campo;
//		delete campo;
//
//		cout<<campoImprimir1<<endl;

		cantRegLeidos++;
		tamanioTotal += sizeof(regVariableLeido);

		if (cantRegLeidos > 1) //tengo algo dentro del buffer
		{
//			RegistroClave* bufferAuxiliar = new RegistroClave[cantRegLeidos - 1];
//			for (int i=0;i<cantRegLeidos-1;i++)
//			{
//				RegistroClave copia(bufferOrdenamiento[i]);
//				bufferAuxiliar[i]=copia;
//			}
//			delete[] bufferOrdenamiento;
//			bufferOrdenamiento = new RegistroClave[cantRegLeidos-1];

//			bufferOrdenamiento = bufferAuxiliar;//fixme original

//			delete[] bufferAuxiliar;

			bufferOrdenamiento[cantRegLeidos-1]=regVariableLeido;//fixme

//			bufferOrdenamiento[cantRegLeidos - 1] = regClaveLeido;//fixme
		} else //no tengo nada dentro
		{
//			bufferOrdenamiento = new RegistroClave[1];
			bufferOrdenamiento[0]= regVariableLeido;
		}
	}
//replacement selection
	Heap heap;
	int tamanioHeapInicial=cantRegLeidos;
	int tamanioHeapActual=tamanioHeapInicial;

	while((archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos)||(cantRunsArmados==0))
	{
		heap.ordenar(bufferOrdenamiento,tamanioHeapActual);

//		for (int i=0;i<tamanioHeapActual;i++)
//		{
//			char* campo=new char[bufferOrdenamiento[i].get_tamanio_campo(0)+1]; //todo borrar
//			bufferOrdenamiento[i].recuperar_campo(campo,0);
//			campo[bufferOrdenamiento[i].get_tamanio_campo(0)]='\0';
//			string campoImprimir1 = campo;
//			delete campo;
//
//			cout<<campoImprimir1<<endl;
//		}

		//obtengo el numero de run
		std::string numeroRun;
		std::stringstream out;
		out << cantRunsArmados;
		numeroRun = out.str();
		//creo el archivo temporal y guardo nombre en vector de runs
		string nombreDelRun=nombreGeneralDeRun+numeroRun+extension;
		archivosTemporalesAFusionar.push_back(nombreDelRun);

		ManejadorRegistrosVariables archivoTemporal;

		archivoTemporal.eliminar_archivo(nombreDelRun);//solo por si acaso hay runs anteriores las borra

		archivoTemporal.crear_archivo(nombreDelRun);
		archivoTemporal.abrir_archivo(nombreDelRun);
		//si no se acaba el archivo, y tengo por lo menos 1 run, hago replacement selection
		while (tamanioHeapActual>0)//si el heap no esta vacio
		{
			if (archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos)
			{
				archivoAOrdenar.get_registro_ocupado(&regVariableLeido,cantRegLeidos);
				cantRegLeidos++;

				if ((heap.comparar_registros_variables(regVariableLeido,bufferOrdenamiento[0])==1)||(heap.comparar_registros_variables(regVariableLeido,bufferOrdenamiento[0])==0))//es decir mayor que la raiz
				{
					archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
					RegistroVariable copia1(regVariableLeido);
					bufferOrdenamiento[0]=copia1;
					heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
				}else
				{
					int ultimaPosicionDelHeap = tamanioHeapActual-1;
					archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
					RegistroVariable copia1(bufferOrdenamiento[ultimaPosicionDelHeap]);
					bufferOrdenamiento[0]=copia1;
					RegistroVariable copia2(regVariableLeido);
					bufferOrdenamiento[ultimaPosicionDelHeap]=copia2;
					tamanioHeapActual--;
					heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
				}
			}else
			{
				int ultimaPosicionDelHeap = tamanioHeapActual-1;
				archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
				RegistroVariable copia1(bufferOrdenamiento[ultimaPosicionDelHeap]);
				bufferOrdenamiento[0]=copia1;
				tamanioHeapActual--;
				heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
			}
		}

		string claveMax;
		claveMax = CLAVE_TOPE;

		RegistroVariable regClaveMax;

		regClaveMax.agregar_campo(claveMax.c_str(), claveMax.length());
		archivoTemporal.agregar_registro(& regClaveMax);

		tamanioHeapActual=tamanioHeapInicial;

//		delete[] bufferOrdenamiento;
		cantRunsArmados++;
	}
}

void SortExterno::_merge()
{
	string claveMax;
	claveMax = CLAVE_TOPE;

	RegistroVariable regClaveMax;
	regClaveMax.agregar_campo(claveMax.c_str(), claveMax.length());

	Heap heap;

	int tamanio = archivosTemporalesAFusionar.size();
	int numeroDeMerge=0;
	//agrego un run "vacio" para fsionar contra ese, poco eficiente, pero soluciona mi problema rapido
	if (tamanio==1)
	{
		string nombreDelRun="run1.dat";

		ManejadorRegistrosVariables archivoTemporal;
		archivoTemporal.eliminar_archivo(nombreDelRun);//solo por si acaso hay runs anteriores las borr
		archivoTemporal.crear_archivo(nombreDelRun);
		archivoTemporal.abrir_archivo(nombreDelRun);

		archivoTemporal.agregar_registro(&regClaveMax);

		archivosTemporalesAFusionar.push_back(nombreDelRun);
		tamanio++;
	}


	//cuando quedan 2 a fusionar lo hago sobre el archivo original
	while (tamanio > 2)
	{
//		cout<<"tamanio:  "<<tamanio<<endl;//todo

		ManejadorRegistrosVariables PrimerArchivoAUnir,SegundoArchivoAUnir;
		PrimerArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-1]); //empiezo de atras para delante
		SegundoArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-2]);

		cout<<archivosTemporalesAFusionar[tamanio-1]<<endl;//todo
		cout<<archivosTemporalesAFusionar[tamanio-2]<<endl;//todo

		//genero archivo donde va la fusion
		std::string rutaMerge;

		ManejadorRegistrosVariables archivoMerge;

		std::string numeroMerge;
		std::stringstream out;
		out << numeroDeMerge;
		numeroMerge = out.str();

//		cout<<"numeroMerge : "<<numeroMerge<<endl;//todo

		rutaMerge= "Merge"+numeroMerge+".dat";

		archivoMerge.crear_archivo(rutaMerge);
		archivoMerge.abrir_archivo(rutaMerge);

		unsigned int indicePrimero=0;
		unsigned int indiceSegundo=0;

		RegistroVariable regPrimero,regSegundo;

		PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
		SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);

		//todo extraer a funcion
		while ( heap.comparar_registros_variables(regPrimero,regClaveMax)==-1 || heap.comparar_registros_variables(regSegundo,regClaveMax)==-1 )
		{

			//comparo y avanzo en el correspondiente
			if (heap.comparar_registros_variables(regPrimero,regSegundo)==0 || heap.comparar_registros_variables(regPrimero,regSegundo)==-1)
			{
				archivoMerge.agregar_registro(&regPrimero);
				indicePrimero++;
				PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
			}else{
				archivoMerge.agregar_registro(&regSegundo);
				indiceSegundo++;
				SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);
			}
		}

		archivoMerge.agregar_registro(&regClaveMax);

		//borrar los dos archivos y agregar el de merge al vector en la posicion 0.
		archivosTemporalesAFusionar.insert(archivosTemporalesAFusionar.begin(), rutaMerge);
		tamanio++;

		PrimerArchivoAUnir.eliminar_archivo(archivosTemporalesAFusionar[tamanio-1]);
		SegundoArchivoAUnir.eliminar_archivo(archivosTemporalesAFusionar[tamanio-2]);

		archivosTemporalesAFusionar.pop_back();//saco los que uni y borre de disco
		archivosTemporalesAFusionar.pop_back();
		tamanio--;//saco 2 y meto 1
		tamanio--;

		numeroDeMerge++;

	}

	//fusiono los ultimos 2 sobre el original

	ManejadorRegistrosVariables PrimerArchivoAUnir,SegundoArchivoAUnir;
	PrimerArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-1]); //empiezo de atras para delante
	SegundoArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-2]);

	//borro el original para sobreescribirlo con el ordenado
	ManejadorRegistrosVariables archivoMerge;

	archivoMerge.abrir_archivo(archAOrdenar);
	archivoMerge.eliminar_archivo(archAOrdenar);

	archivoMerge.crear_archivo(archAOrdenar);
	archivoMerge.abrir_archivo(archAOrdenar);

	unsigned int indicePrimero=0;
	unsigned int indiceSegundo=0;

	RegistroVariable regPrimero,regSegundo;

	PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
	SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);

	while ( heap.comparar_registros_variables(regPrimero,regClaveMax)==-1 || heap.comparar_registros_variables(regSegundo,regClaveMax)==-1 )
	{

		//comparo y avanzo en el correspondiente
		if (heap.comparar_registros_variables(regPrimero,regSegundo)==0 || heap.comparar_registros_variables(regPrimero,regSegundo)==-1)
		{
			archivoMerge.agregar_registro(&regPrimero);
			indicePrimero++;
			PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
		}else{
			archivoMerge.agregar_registro(&regSegundo);
			indiceSegundo++;
			SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);
		}
	}

	PrimerArchivoAUnir.eliminar_archivo(archivosTemporalesAFusionar[tamanio-1]);
	SegundoArchivoAUnir.eliminar_archivo(archivosTemporalesAFusionar[tamanio-2]);

}

void SortExterno::ordenar_archivo(string nombreArchivo)
{
	_generar_runs();
	_merge();
}
