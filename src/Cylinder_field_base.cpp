#include "../include/Cylinder_field_base.h"
#include <fstream>
#include <sstream>
#include <String>
#include <iomanip>
using namespace std;

namespace Common {
	void CDumpEHxyz(const vector<vector<complex<double>>>& E, std::string& buffer, size_t n, size_t m) {
		if (!E.empty()) {
			bool flag = true;
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			double real, imag;
			for (size_t i = 0; i < n; i++) {
				for (size_t j = 0; j < m; j++) {
					real = E[i][j].real();  imag = E[i][j].imag();
					buffer.append(reinterpret_cast<const char*>(&real), sizeof(double));
					buffer.append(reinterpret_cast<const char*>(&imag), sizeof(double));
				}
			}
		}
		else {
			bool flag = false;
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
		}
	}

	int CLoadEHxyz(vector<vector<complex<double>>>&E, const char*& p, size_t n, size_t m, size_t& size) {
		bool flag = false;
		if (size < sizeof(size_t)) return -1;
		memcpy(&flag, p, sizeof(flag));
		p += sizeof(bool);
		size -= sizeof(bool);
		if (flag) {
			if (size < n * m * sizeof(double) * 2) return -1;
			CylinderFieldBase::Resize(E, n, m);
			double real, imag;
			for (size_t i = 0; i < n; i++) {
				for (size_t j = 0; j < m; j++) {
					memcpy(&real, p, sizeof(double));
					p += sizeof(double);
					memcpy(&imag, p, sizeof(double));
					p += sizeof(double);
					E[i][j] = complex<double>(real, imag);
				}
			}
			size -= n * m * sizeof(double) * 2;
		}
		return 0;
	}

	int CylinderFieldBase::writeFieldBase(const std::string & path)
	{

		return 0;
	}

	int CylinderFieldBase::readFieldBase(const std::string & path)
	{
		return 0;
	}

	int CylinderFieldBase::writeFieldBaseASCII(const std::string & path)
	{
		ofstream outfile(path);
		stringstream temp;
		int Nphi = Ephi.size();	int Nz = Ephi[0].size();
		double rad2deg = 180 / 3.1415926535898;
		temp << coor.pos.x << " "
			<< coor.pos.y << " "
			<< coor.pos.z << " "
			<< coor.rotate_axis.x << " "
			<< coor.rotate_axis.y << " "
			<< coor.rotate_axis.z << " "
			<< coor.rotate_theta << " "
			<< Nphi << " " << Nz << " " << ds_phi << " " << ds_z << " " << centerphi << " " << centerz << " " << R <<" "
			<< 1 <<" "<< frequency << endl;
		for (int i = 0; i < Nphi; i++)
			for (int j = 0; j < Nz; j++)
			{
				temp
					<< abs(Ephi[i][j]) << " " << arg(Ephi[i][j]) * rad2deg << " "
					<< abs(Ez[i][j]) << " " << arg(Ez[i][j]) * rad2deg << " "
					<< abs(Er[i][j]) << " " << arg(Er[i][j]) * rad2deg << " "
					<< abs(Hphi[i][j]) << " " << arg(Hphi[i][j]) * rad2deg << " "
					<< abs(Hz[i][j]) << " " << arg(Hz[i][j]) * rad2deg << " "
					<< abs(Hr[i][j]) << " " << arg(Hr[i][j]) * rad2deg << " "
					<< endl;
			}
		outfile << temp.str() << endl;
		outfile.close();
		return 0;
	}

	int CylinderFieldBase::readFieldBaseASCII(const std::string & path)
	{
		const double Pi = 3.1415926535898;
		int flag; double freq;
		ifstream file(path);
		stringstream tempfile;
		tempfile << file.rdbuf();
		file.close();

		string temp;
		getline(tempfile, temp);
		istringstream tempLine(temp);

		double tx, ty, tz, rx, ry, rz, rth;
		tempLine >> tx >> ty >> tz >> rx >> ry >> rz >> rth;
		coor.pos = Vector3(tx, ty, tz);
		coor.SetRotate(Vector3(rx, ry, rz), rth);

		int Numphi, Numz;
		tempLine >> Numphi >> Numz >> ds_phi >> ds_z >> centerphi >> centerz >> R;

		tempLine >> flag >> freq;	
		frequency = freq;//´«¸øCylinderField_Base

		Resize(Ephi, Numphi, Numz);
		Resize(Ez, Numphi, Numz);
		Resize(Er, Numphi, Numz);
		Resize(Hphi, Numphi, Numz);
		Resize(Hz, Numphi, Numz);
		Resize(Hr, Numphi, Numz);

		if (flag == 0) {	//Source Only

			double a1, p1, a2, p2, a3, p3, a4, p4, a5, p5, a6, p6;
			for (int i = 0; i < Numphi; i++)
			{
				for (int j = 0; j < Numz; j++)
				{
					getline(tempfile, temp);
					istringstream tempLine(temp);


					tempLine >> a1 >> p1 >> a2 >> p2;

					Ephi[i][j] = complex<double>(a1*cos(p1 / 180 * Pi), a1*sin(p1 / 180 * Pi));
					Ez[i][j] = complex<double>(a2*cos(p2 / 180 * Pi), a2*sin(p2 / 180 * Pi));

				}
			}
		}
		else if (flag == 1) {	//All Fields
			double a1, p1, a2, p2, a3, p3, a4, p4, a5, p5, a6, p6;
			for (int i = 0; i < Numphi; i++)
			{
				//cout << i << " ";
				for (int j = 0; j < Numz; j++)
				{
					getline(tempfile, temp);
					istringstream tempLine(temp);
					
					tempLine >> a1 >> p1 >> a2 >> p2 >> a3 >> p3 >> a4 >> p4 >> a5 >> p5 >> a6 >> p6;

					Ephi[i][j] = complex<double>(a1*cos(p1 / 180 * Pi),	a1*sin(p1 / 180 * Pi));
					Ez[i][j] = complex<double>(a2*cos(p2 / 180 * Pi),	a2*sin(p2 / 180 * Pi));
					Er[i][j] = complex<double>(a3*cos(p3 / 180 * Pi),	a3*sin(p3 / 180 * Pi));
					Hphi[i][j] = complex<double>(a4*cos(p4 / 180 * Pi),	a4*sin(p4 / 180 * Pi));
					Hz[i][j] = complex<double>(a5*cos(p5 / 180 * Pi),	a5*sin(p5 / 180 * Pi));
					Hr[i][j] = complex<double>(a6*cos(p6 / 180 * Pi),	a6*sin(p6 / 180 * Pi));

				}
			}
		}
		else {
			return -1;
		}

		return 0;
	}

	void CylinderFieldBase::Resize(vector<vector<complex<double>>>& E, size_t n, size_t m)
	{
		E.resize(n);
		for (size_t i = 0; i < n; i++) {
			E[i].resize(m);
		}
	}
}
