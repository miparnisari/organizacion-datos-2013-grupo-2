#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

#include <string>
#include "../../Constantes.h"
#include "../../CapaFisica/RegistroVariable.h"

class ArbolBMas
{
	public:
		ArbolBMas();
		int create (std::string dir, std::string fileName);
		int open (std::string dir, std::string fileName, std::string mode);
		int destroy (std::string dir, std::string fileName);
		int close ();
		
		int agregar(RegistroVariable & reg);
		virtual ~ArbolBMas();
	
	private:
		/* add your private declarations */
};

#endif /* ARBOLBMAS_H */ 
