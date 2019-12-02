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

		size_t Size() const;
		int Open(const std::string& file_name);
		int Save(const std::string& file_name) const;

		void Serialize(std::string& buffer) const;
		int UnSerialize(const char* p, size_t size);
		int UnSerialize(const std::string& buffer);

		static void Resize(vector<vector<complex<double>>>& E, size_t n, size_t m);
	};
}