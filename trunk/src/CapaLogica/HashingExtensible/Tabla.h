#ifndef TABLA_H
#define TABLA_H


class Tabla
{
	private:
		/* add your declarations */

	public:

        Tabla();

        Tabla(char *c_tabla);

        ~Tabla();

        virtual int modificar_pos(int pos, int dato);

        virtual int tam();

        virtual int obtener_valor(int pos);

        virtual char *devolver_string();

        virtual int mover_der(int cantPos);

        virtual int mover_izq(int cantPos);

        virtual int duplicar_tabla();
		/* add your declarations */

};

#endif // TABLA_H
