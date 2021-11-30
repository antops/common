#pragma once
#include "coordinate.h"
#include <string>
#include <vector>
#include <complex>
using std::complex;
using std::vector;
namespace Common {
	struct CylinderFieldBase
	{
		vector<vector<complex<double>>> Ephi;
		vector<vector<complex<double>>> Ez;
		vector<vector<complex<double>>> Er;
		vector<vector<complex<double>>> Hphi;
		vector<vector<complex<double>>> Hz;
		vector<vector<complex<double>>> Hr;

		Coordinate coor;

		// Ä¬ÈÏds_x = ds_y
		double ds_phi;
		double ds_z;
		double centerz;
		double centerphi;
		double R;

		double frequency;

		void Clear() {
			Ephi.clear();
			Ez.clear();
			Er.clear();
			Hphi.clear();
			Hz.clear();
			Hr.clear();
		}

		size_t Size() const;
		int Open(const std::string& file_name);
		int Save(const std::string& file_name) const;

		void Serialize(std::string& buffer) const;
		int UnSerialize(const char* p, size_t size);
		int UnSerialize(const std::string& buffer);

		int writeFieldBase(const std::string& path);
		int readFieldBase(const std::string& path);

		int writeFieldBaseASCII(const std::string& path);
		int readFieldBaseASCII(const std::string& path);

		static void Resize(vector<vector<complex<double>>>& E, size_t n, size_t m);
	};
}
