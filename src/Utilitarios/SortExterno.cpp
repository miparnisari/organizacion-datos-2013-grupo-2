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
	//if res_error ya veremos TODO
	//if no registros ya veremos

	string nombreGeneralDeRun="run";
	string extension=".dat";
	int cantRunsArmados=0;

	int cantRegLeidos=0;
	unsigned tamanioTotal=0;//del vector a ordenar por heapsort en RAM

	RegistroClave regClaveLeido;
//	RegistroClave* bufferOrdenamiento;
	RegistroClave bufferOrdenamiento[1000];//fixme

	//lleno el heap por primera vez
	//puede que resulta un poco mayor al limite (lo ultimo que leo se puede pasar)
	while ((tamanioTotal < TAM_MAX_BUFFER_ORDENAMIENTO)&&(archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos))
	{
		archivoAOrdenar.get_registro_ocupado(&regClaveLeido, cantRegLeidos);
		cantRegLeidos++;
		tamanioTotal += sizeof(regClaveLeido);//esto no creo q este bien pero como indicador servira por ahora

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

			bufferOrdenamiento[cantRegLeidos-1]=regClaveLeido;//fixme

//			bufferOrdenamiento[cantRegLeidos - 1] = regClaveLeido;//fixme falla aca!
		} else //no tengo nada dentro
		{
//			bufferOrdenamiento = new RegistroClave[1];
			bufferOrdenamiento[0]= regClaveLeido;
		}
	}
//replacement selection
	Heap heap;
	int tamanioHeapInicial=cantRegLeidos;
	int tamanioHeapActual=tamanioHeapInicial;

	while((archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos)||(cantRunsArmados==0))
	{
		heap.ordenar(bufferOrdenamiento,tamanioHeapActual);

		//obtengo el numero de run
		std::string numeroRun;
		std::stringstream out;
		out << cantRunsArmados;
		numeroRun = out.str();
		//creo el archivo temporal y guardo nombre en vector de runs
		string nombreDelRun=nombreGeneralDeRun+numeroRun+extension;
		archivosTemporalesAFusionar.push_back(nombreDelRun);

		ManejadorRegistrosVariables archivoTemporal;

		archivoTemporal.crear_archivo(nombreDelRun);
		archivoTemporal.abrir_archivo(nombreDelRun);
		//si no se acaba el archivo, y tengo por lo menos 1 run, hago replacement selection
		while (tamanioHeapActual>0)//si el heap no esta vacio
		{
			if (archivoAOrdenar.get_cantidad_registros_ocupados()>cantRegLeidos)
			{
				archivoAOrdenar.get_registro_ocupado(&regClaveLeido,cantRegLeidos);
				cantRegLeidos++;


				if (regClaveLeido >= bufferOrdenamiento[0])//es decir mayor que la raiz
				{//fixme ver si es > o <!!!
					archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
					RegistroClave copia1(regClaveLeido);
					bufferOrdenamiento[0]=copia1;
					heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
				}else
				{
					int ultimaPosicionDelHeap = tamanioHeapActual-1; //todo metodo agregar al Heap
					archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
					RegistroClave copia1(bufferOrdenamiento[ultimaPosicionDelHeap]);
					bufferOrdenamiento[0]=copia1;
					RegistroClave copia2(regClaveLeido);
					bufferOrdenamiento[ultimaPosicionDelHeap]=copia2;
					tamanioHeapActual--;
					heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
				}
			}else
			{
				int ultimaPosicionDelHeap = tamanioHeapActual-1; //todo metodo agregar al Heap
				archivoTemporal.agregar_registro(&bufferOrdenamiento[0]);
				RegistroClave copia1(bufferOrdenamiento[ultimaPosicionDelHeap]);
				bufferOrdenamiento[0]=copia1;
				tamanioHeapActual--;
				heap.ordenar(bufferOrdenamiento,tamanioHeapActual);
			}
		}

		ClaveX clave;
		clave.set_clave(200); //todo cambiar a clave tope
		RegistroClave regClave;
		regClave.set_clave(clave);
		archivoTemporal.agregar_registro(& regClave);

		tamanioHeapActual=tamanioHeapInicial;

//		delete[] bufferOrdenamiento;
		cantRunsArmados++;
	}
}

void SortExterno::_merge()
{

	RegistroClave regClaveMax;

	ClaveX clave;
	clave.set_clave(200);//todo cambiar a CLAVE_TOPE

	regClaveMax.set_clave(clave);

	int tamanio = archivosTemporalesAFusionar.size();
	int numeroDeMerge=0;
	//cuando quedan 2 a fusionar lo hago sobre el archivo original
	while (tamanio > 2)
	{
//		cout<<"tamanio:  "<<tamanio<<endl;//todo

		ManejadorRegistrosVariables PrimerArchivoAUnir,SegundoArchivoAUnir;
		PrimerArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-1]); //empiezo de atras para delante
		SegundoArchivoAUnir.abrir_archivo(archivosTemporalesAFusionar[tamanio-2]);

		unsigned short indicePrimero=0;
		unsigned short indiceSegundo=0;

		RegistroClave regPrimero,regSegundo;

		std::string rutaMerge;

		PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
		SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);
//todo extraer a funcion
		ManejadorRegistrosVariables archivoMerge;

		while ( (regPrimero < regClaveMax) && (regSegundo < regClaveMax) )
		{
//			cout<<"par de claves: "<<endl; //todo
//			regPrimero.get_clave().imprimir_dato();
//			cout<<endl;
//			regSegundo.get_clave().imprimir_dato();
//			cout<<endl;

			//genero archivo donde va la fusion

			std::string numeroMerge;
			std::stringstream out;
			out << numeroDeMerge;
			numeroMerge = out.str();

//			cout<<"numeroMerge : "<<numeroMerge<<endl;//todo

			rutaMerge= "Merge"+numeroMerge+".dat";

			archivoMerge.crear_archivo(rutaMerge);
			archivoMerge.abrir_archivo(rutaMerge);

			//comparo y avanzo en el correspondiente
			if (regPrimero <= regSegundo)
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

//	archivoMerge.abrir_archivo(archAOrdenar);
//	archivoMerge.eliminar_archivo(archAOrdenar);

	archivoMerge.crear_archivo("ordenado.dat");//fixme aca va arch a ordeanr
	archivoMerge.abrir_archivo("ordenado.dat");

	unsigned short indicePrimero=0;
	unsigned short indiceSegundo=0;

	RegistroClave regPrimero,regSegundo;

	PrimerArchivoAUnir.get_registro_ocupado(&regPrimero,indicePrimero);
	SegundoArchivoAUnir.get_registro_ocupado(&regSegundo,indiceSegundo);


	while ( (regPrimero < regClaveMax) && (regSegundo < regClaveMax ) )
	{
//		cout<<"------------------------- ULTIMO MERGE ------------------------"<<endl; //todo
//
//		cout<<"par de claves: "<<endl; //todo
//		regPrimero.get_clave().imprimir_dato();
//		cout<<endl;
//		regSegundo.get_clave().imprimir_dato();
//		cout<<endl;

		//comparo y avanzo en el correspondiente
		if (regPrimero <= regSegundo)
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
