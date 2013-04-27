#ifndef ANIO_H_
#define ANIO_H_

#include<string>
#include<cstdlib>

using namespace std;

class Anio {

	protected:
		int anio;

	public:
		static int const ANIO_DEFAULT= -1;

		bool validar(string anio);
		/*valida que un anio sea correcto*/
		bool cargar(string anioS);
		/*carga un anio, se valida, se almacena.*/
		int get_anio();

		void resetear();


		Anio();

};

#endif /* ANIO_H_ */
