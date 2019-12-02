#pragma once
#include <iostream>
#include <fstream>
#include "../common/include/field_base.h"

using Common::FieldBase;

int FuncTestFieldBase() {
	FieldBase field;
	field.ds_x = 1.0;
	field.ds_y = 2.0;
	field.frequency = 3e10;
	field.coor.N = Vector3(1, 4, 6);

	int n = 100;
	int m = 100;
	auto& Ex = field.Ex;
	auto& Ey = field.Ey;
	field.Resize(Ex, 100, 100);
	field.Resize(Ey, 100, 100);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Ex[i][j] = complex<double>(i, j);
			Ey[i][j] = complex<double>(j, i);
		}
	}

	field.Save("./data.dat");


	FieldBase field2;
	field2.Open("./data.dat");

	std::cout << field2.ds_x << std::endl;
	std::cout << field.coor.N.Debug() << std::endl;
	std::cout << field2.frequency << std::endl;

	std::cout << field.Ex[0][99] << std::endl;
	std::cout << field.Ey[98][1] << std::endl;
	return 0;
}
