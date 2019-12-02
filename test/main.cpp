#include <iostream>
#include <fstream>
#include "coordinate_test.h"
#include "vector_test.h"
#include "field_base_test.h"

int main() {
	std::string b1("2sd");
	std::string b2 = std::move(b1);
	// FuncTestVector3();
	//FuncTestCoordinate();
	FuncTestFieldBase();
	getchar();
	return 0;
}