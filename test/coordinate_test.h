#pragma once
#include <iostream>
#include <fstream>
#include "../common/include/coordinate.h"


using Common::Vector3;
using Common::Coordinate;

int FuncTestCoordinate() {
	Coordinate coor;
	coor.SetRotate(Vector3(1,0,0), 30);
	std::cout << coor.Debug() << std::endl;

	std::ofstream outFile("./test.dat", std::ios::binary | std::ios::out);
	if (!outFile) {
		std::cout << "outFile error" << std::endl;
		return 0;
	}
	coor.Save(outFile);
	outFile.close();

	std::ifstream inFile("./test.dat", std::ios::in | std::ios::binary);
	if (!inFile) {
		std::cout << "inFile error" << std::endl;
		return 0;
	}
	Coordinate coor1;
	coor1.Open(inFile);
	inFile.close();
	std::cout << coor1.Debug() << std::endl;
	return 0;
}