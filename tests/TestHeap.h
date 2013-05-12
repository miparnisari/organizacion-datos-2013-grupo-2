/*
 * TestHeap.h
 *
 *  Created on: May 11, 2013
 *      Author: martin
 */

#ifndef TESTHEAP_H_
#define TESTHEAP_H_

#include "../src/Utilitarios/Heap.h"
#include"Test.h"

class TestHeap : public Test{

	public:
		TestHeap();
		virtual ~TestHeap();
		void ejecutar();

	private:
		void testHeapsort();

};

#endif /* TESTHEAP_H_ */
