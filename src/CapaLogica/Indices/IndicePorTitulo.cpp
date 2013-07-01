#include "IndicePorTitulo.h"

IndicePorTitulo::IndicePorTitulo()
{
}

IndicePorTitulo::~IndicePorTitulo()
{
}

int IndicePorTitulo::crear_indice(std::string directorioSalida)
{
    this->nombre_archivo = directorioSalida;
    return this->indice.crear_archivo(this->nombre_archivo);
}

int IndicePorTitulo::eliminar_indice()
{
    return this->indice.eliminar_archivo();
}

int IndicePorTitulo::abrir_indice(std::string directorioSalida)
{
    this->nombre_archivo = directorioSalida;
    return this->indice.abrir_archivo(this->nombre_archivo);
}

int IndicePorTitulo::cerrar_indice()
{
	return this->indice.cerrar_archivo();
}

int IndicePorTitulo::agregar(const std::string & titulo, IDdocumento_t IDcancion)
{
	ClaveX claveTitulo;
	claveTitulo.set_clave(titulo);
	RegistroClave reg_clave;

	if (indice.devolver(claveTitulo,&reg_clave) == RES_RECORD_DOESNT_EXIST)
	{
		reg_clave.set_clave(claveTitulo);
		reg_clave.agregar_campo((char*)&IDcancion,sizeof(IDcancion));
		indice.agregar(reg_clave);
	}
	else {
		IDdocumento_t id_doc_ya_existente;
		int cantidad_id_docs = reg_clave.get_cantidad_campos();
		for (int i = 1; i < cantidad_id_docs; i++)
		{
			reg_clave.recuperar_campo((char*)&id_doc_ya_existente,i);
			if (id_doc_ya_existente == IDcancion)
				return RES_RECORD_EXISTS;
		}
		reg_clave.agregar_campo((char*)&IDcancion,sizeof(IDcancion));
		indice.modificar(reg_clave);
	}

	return RES_OK;
}

int IndicePorTitulo::buscar(const std::string & titulo, vector<IDdocumento_t> & id_docs)
{
	ClaveX clave;
	clave.set_clave(titulo);
	id_docs.clear();
	RegistroClave reg_titulo;
	if (indice.devolver(clave,&reg_titulo) == RES_RECORD_DOESNT_EXIST)
	{
		return RES_RECORD_DOESNT_EXIST;
	}
	else {
		IDdocumento_t id_doc_ya_existente;
		int cantidad_id_docs = reg_titulo.get_cantidad_campos();
		for (int i = 1; i < cantidad_id_docs; i++)
		{
			reg_titulo.recuperar_campo((char*)&id_doc_ya_existente,i);
			id_docs.push_back(id_doc_ya_existente);
		}
	}

	return RES_OK;
}

int IndicePorTitulo::eliminar(const std::string & titulo, const IDdocumento_t IDcancion)
{
	ClaveX clave;
	clave.set_clave(titulo);
	RegistroClave reg_titulo;
	reg_titulo.set_clave(clave);
	if (indice.devolver(clave,&reg_titulo) == RES_RECORD_DOESNT_EXIST)
	{
		return RES_RECORD_DOESNT_EXIST;
	}
	else {
		// Copio en un nuevo registro todos los IDs, excepto el que quiero eliminar
		RegistroClave reg_titulo_copia;
		reg_titulo_copia.set_clave(clave);

		IDdocumento_t id_doc_ya_existente;
		int cantidad_id_docs = reg_titulo.get_cantidad_campos();
		for (int i = 1; i < cantidad_id_docs; i++)
		{
			reg_titulo.recuperar_campo((char*)&id_doc_ya_existente,i);
			if (id_doc_ya_existente != IDcancion)
				reg_titulo_copia.agregar_campo((char*)&id_doc_ya_existente,sizeof(int));
		}

		// No quedaron IDs para este titulo
		if (reg_titulo_copia.get_cantidad_campos() == 1) //quedo solo la clave
			return indice.eliminar(clave);


		indice.modificar(reg_titulo_copia);

	}

	return RES_OK;
}
