#ifndef INDICEINVERTIDO_H_INCLUDED
#define INDICEINVERTIDO_H_INCLUDED

#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ManejoArchivos/ClaveX.h"
#include "../../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../../CapaLogica/ManejoArchivos/Texto.h"
#include "ArchivoListas.h"
#include "../../Constantes.h"

class IndiceInvertido
{
    //Atributos
    protected:

        std::string fileName;
        std::string ruta;
        ManejadorRegistrosVariables archivo_terminos;
        ArbolBMas vocabulario;
        ArchivoListas listas_invertidas;
        ArchivoListas listas_posiciones;
        ManejadorRegistrosVariables archivo_coincidencias;

    public:

        IndiceInvertido();

        virtual ~IndiceInvertido();

        virtual int crear_indice(std::string directorioSalida, std::string fileNamee);
        //Crea los archivos necesarios para el indice

        virtual int abrir_indice(std::string directorioSalida, std::string fileNamee);
        //Abre los archivos que necesita el indice

        virtual int cerrar_indice();

        virtual int agregar_cancion(RegistroCancion cancion, int IDcancion);
        //Agrega los terminos de la cancion al indice junto a su posicion

        virtual int borrar_indice();
        //Borra el indice junto a sus archivos

        virtual int buscar_frase(std::string frase, RegistroVariable &lista);
        //Busca aquellas canciones que contienen la frase pasada por parametro y las guarda en la lista

    private:

        virtual int armar_archivo_coincidencias(std::string letra);
        //Guarda en un archivo de coincidencias el (IDter, pos) de cada termino en la letra

        virtual int armar_listas_invertidas(int IDcancion);
        //Guarda en la lista invertida de cada termino una lista con las posiciones de esta cancion

        virtual int interseccion_listas_invertidas(std::string frase, RegistroVariable &canciones);
        //Guarda en canciones aquellas letras que contienen todos los terminos de la frase

        virtual int interseccion(RegistroVariable &canciones, RegistroVariable &listaAux);
        //Busco la interseccion de las dos listas y la guardo en la lista canciones

        virtual int obtener_canciones_termino(const char *termino, RegistroVariable &canciones);
        //Guarda en la lista de canciones todas aquellas canciones en las que aparece el termino pasado por parametro

        virtual int armar_archivo_terminos_frase(std::string frase, RegistroVariable canciones, RegistroVariable &terminos_frase);
        //Arma un archivo termporal con registros (IDcancion, pos, IDtermino) que va a servir para buscar las canciones que tienen la frase incluida

        virtual int buscar_cancion_en_lista(int IDcancion, RegistroVariable &canciones);
        //Busca dentro de la la lista de canciones si se encuenta la cancion pasada por parametro

        virtual int buscar_cancion_con_frase(RegistroVariable terminos_frase, RegistroVariable &lista);
        //Busca dentro del archivo temporal que documentos tienen los terminos en el orden de la frase

        virtual int siguiente_termino_frase(int &pos_reg, int pos_ter_frase, int &posAnterior, int &IDcancionAnterior, RegistroVariable &terminos_frase);
        //Es recursiva, sirve para buscar si dentro de una cancion se encuentra los terminos en el orden de la frase, en caso que no lo este devuelve NO_PERTENECE

};

#endif // INDICEINVERTIDO_H_INCLUDED
