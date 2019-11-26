#pragma once
#include <iostream>
#include <fstream>
#include "../common/include/Vector3.h"

using Common::Vector3;

int FuncTestVector3() {
	Vector3 v1(1, 2, 4);
	std::cout << v1.Debug() << std::endl;
	std::string buffer = v1.Serialize();
	Vector3 v2(4, 5, 6);
	std::cout << v2.Debug() << std::endl;
	v2.UnSerialize(buffer);
	std::cout << v2.Debug() << std::endl;

	std::ofstream outFile("./test.dat", std::ios::binary | std::ios::out);
	if (!outFile) {
	std::cout << "outFile error" << std::endl;
	return 0;
	}
	v1.Save(outFile);
	outFile.close();

	std::ifstream inFile("./test.dat", std::ios::in | std::ios::binary);
	if (!inFile) {
		std::cout << "inFile error" << std::endl;
		return 0;
	}

	Vector3 v3;
	v3.Open(inFile);
	std::cout << v3.Debug() << std::endl;
	inFile.close();
	return 0;
}