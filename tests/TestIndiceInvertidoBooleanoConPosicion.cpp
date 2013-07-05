#include "../src/CapaLogica/Indices/IndiceInvertidoBooleanoConPosicion.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

#include <vector>

class TestIndiceInvertidoBooleanoConPosicion : public testing::Test {
 protected:
	IndiceInvertidoBooleanoConPosicion indice;
	Documento_t doc1, doc2, doc3, doc4;
	IDdocumento_t id_doc1, id_doc2, id_doc3, id_doc4;

  virtual void SetUp() {
	  ASSERT_EQ(indice.crear("indice_invertido"), RES_OK);
	  ASSERT_EQ(indice.abrir("indice_invertido"), RES_OK);
	  doc1 = "north korea and \n south korea to to to to to to to to to to to to to to to to to to to to to to to to to to to to to to to to";
	  doc2 = "south korea is \n capitalist to to to to to to to to to to to to to to to to";
	  doc3 = "war between south and north korea \n 1952 19666 to to to to to to to to to to to to to to to to to to to to to to to to";
	  doc4 = "Harry Truman, Doris Day, Red China, Johnny Ray \n South Pacific, Walter Winchell, Joe DiMaggio \n Joe McCarthy, Richard Nixon, Studebaker, Television \n North Korea, South Korea, Marilyn Monroe";
	  id_doc1 = 1;
	  id_doc2 = 2;
	  id_doc3 = 3;
	  id_doc4 = 4;
  }

  virtual void TearDown() {
	  ASSERT_EQ(indice.cerrar(),RES_OK);
	  ASSERT_EQ(indice.eliminar(), RES_OK);
  }

};


TEST_F(TestIndiceInvertidoBooleanoConPosicion,Busqueda_frases)
{
	ASSERT_EQ(indice.agregar(doc1,id_doc1),RES_OK);
	ASSERT_EQ(indice.agregar(doc2,id_doc2),RES_OK);
	ASSERT_EQ(indice.agregar(doc3,id_doc3),RES_OK);

	ASSERT_EQ(indice.finalizar_agregado(),RES_OK);

	std::vector<IDdocumento_t> id_docs;

	indice.buscar_frase("",id_docs);
	ASSERT_EQ(id_docs.size(),0);

	indice.buscar_frase("north",id_docs);
	ASSERT_EQ(id_docs.size(),2);
	ASSERT_EQ(id_docs[0],id_doc1);
	ASSERT_EQ(id_docs[1],id_doc3);

	indice.buscar_frase("south",id_docs);
	ASSERT_EQ(id_docs.size(),3);
	ASSERT_EQ(id_docs[0],id_doc1);
	ASSERT_EQ(id_docs[1],id_doc2);
	ASSERT_EQ(id_docs[2],id_doc3);


	indice.buscar_frase("north korea", id_docs);
	ASSERT_EQ(id_docs.size(),2);
	ASSERT_EQ(id_docs[0],id_doc1);
	ASSERT_EQ(id_docs[1],id_doc3);

	indice.buscar_frase("south korea is capitalist", id_docs);
	ASSERT_EQ(id_docs.size(),1);
	ASSERT_EQ(id_docs[0],id_doc2);

	// Hay un \n de por medio
	indice.buscar_frase("and south korea", id_docs);
	ASSERT_EQ(id_docs.size(),1);
	ASSERT_EQ(id_docs[0],id_doc1);

	indice.buscar_frase("1952", id_docs);
	ASSERT_EQ(id_docs.size(), 1);
	ASSERT_EQ(id_docs[0],id_doc3);

	// Se encontrará el doc 3 porque se indexó "1966", No se indexan numeros mayores a LONGITUD_MAXIMA_NUMERO_INDEXABLE = 4;
	indice.buscar_frase("19666", id_docs);
	ASSERT_EQ(id_docs.size(), 1);
	ASSERT_EQ(id_docs[0],id_doc3);

	indice.buscar_frase("to", id_docs);
	ASSERT_EQ(id_docs.size(),3);
	ASSERT_EQ(id_docs[0],id_doc1);
	ASSERT_EQ(id_docs[1],id_doc2);
	ASSERT_EQ(id_docs[2],id_doc3);

	indice.buscar_frase("una frase muy muy larga que no esta en ninguno de los indices", id_docs);
	ASSERT_EQ(id_docs.size(),0);

	indice.buscar_frase("to to to to to to to", id_docs);
	ASSERT_EQ(id_docs.size(),3);
	ASSERT_EQ(id_docs[0],id_doc1);
	ASSERT_EQ(id_docs[1],id_doc2);
	ASSERT_EQ(id_docs[2],id_doc3);
}

TEST_F(TestIndiceInvertidoBooleanoConPosicion, Busqueda_un_doc)
{
	ASSERT_EQ(indice.agregar(doc4,id_doc4),RES_OK);

	ASSERT_EQ(indice.finalizar_agregado(),RES_OK);

	std::vector<IDdocumento_t> id_docs;
	indice.buscar_frase("korea marilyn", id_docs);
	ASSERT_EQ(id_docs.size(),1);
	ASSERT_EQ(id_docs[0],id_doc4);
}
