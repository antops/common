#pragma once
#include "coordinate.h"
#include <string>
#include <vector>
#include <complex>
using std::complex;
using std::vector;
namespace Common {
	struct FieldBase
	{
		vector<vector<complex<double>>> Ex;
		vector<vector<complex<double>>> Ey;
		vector<vector<complex<double>>> Ez;
		vector<vector<complex<double>>> Hx;
		vector<vector<complex<double>>> Hy;
		vector<vector<complex<double>>> Hz;

		Coordinate coor;

		// Ä¬ÈÏds_x = ds_y
		double ds_x;
		double ds_y;

		double frequency;

		void Clear() {
			Ex.clear();
			Ey.clear();
			Ez.clear();
			Hx.clear();
			Hy.clear();
			Hz.clear();
		}

		int writeFieldBase(const std::string& path);
		int readFieldBase(const std::string& path);

		int writeFieldBaseASCII(const std::string& path);
		int readFieldBaseASCII(const std::string& path);

		static void Resize(vector<vector<complex<double>>>& E, size_t n, size_t m);
	};
}