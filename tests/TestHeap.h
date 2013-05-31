/*
 * TestHeap.h
 *
 *  Created on: May 11, 2013
 *      Author: martin
 */

#ifndef TESTHEAP_H_
#define TESTHEAP_H_

#include"Test.h"

class TestHeap : public Test{

	public:
		TestHeap();
		virtual ~TestHeap();
		void ejecutar();

	private:
		void test_ordenar_registros();
		void test_comparar_reg_variables_string();


};

#endif /* TESTHEAP_H_ */
