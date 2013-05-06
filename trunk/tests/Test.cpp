/*
 * Test.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "Test.h"

Test::Test() {

}

Test::~Test() {
}

void Test::print_test_ok(std::string nombreTest)
{
	std::cout << "OK: " << nombreTest << std::endl;
}

