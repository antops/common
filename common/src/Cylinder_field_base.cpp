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

	size_t CylinderFieldBase::Size() const
	{
		size_t size = coor.Size()
			+ 6 * sizeof(bool)
			+ 2 * sizeof(size_t)
			+ sizeof(ds_phi)
			+ sizeof(ds_z)
			+ sizeof(frequency);
		if (Ephi.empty()) return size;
		size_t n = Ephi.size();
		size_t m = Ephi[0].size();
		if (!Ephi.empty()) size += n * m * sizeof(double) * 2;
		if (!Ez.empty()) size += n * m * sizeof(double) * 2;
		if (!Er.empty()) size += n * m * sizeof(double) * 2;
		if (!Hphi.empty()) size += n * m * sizeof(double) * 2;
		if (!Hz.empty()) size += n * m * sizeof(double) * 2;
		if (!Hr.empty()) size += n * m * sizeof(double) * 2;

		return size;
	}

	int CylinderFieldBase::Open(const std::string & file_name)
	{
		std::ifstream in(file_name);
		if (!in.is_open()) return -1;
		std::string buffer((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		in.close();
		return UnSerialize(buffer);
	}

	int CylinderFieldBase::Save(const std::string & file_name) const
	{
		std::ofstream out(file_name);
		if (!out.is_open()) return -1;
		std::string buffer;
		Serialize(buffer);
		out.write(buffer.c_str(), buffer.size());
		out.close();
		return 0;

	}

	void CylinderFieldBase::Serialize(std::string& buffer) const
	{
		std::cout << "coor.Size():" << coor.Size() << std::endl;
		std::cout << "size:" << Size() << std::endl;
		buffer.reserve(Size());
		coor.Serialize(buffer);
		if (Ephi.empty()) {
			bool flag = false;
			size_t n = 0;
			size_t m = 0;
			buffer.append(reinterpret_cast<const char*>(&n), sizeof(n));
			buffer.append(reinterpret_cast<const char*>(&m), sizeof(m));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
			buffer.append(reinterpret_cast<const char*>(&flag), sizeof(flag));
		}
		else {
			size_t n = Ephi.size();
			size_t m = Ephi[0].size();
			buffer.append(reinterpret_cast<const char*>(&n), sizeof(n));
			buffer.append(reinterpret_cast<const char*>(&m), sizeof(m));
			CDumpEHxyz(Ephi, buffer, n, m);
			CDumpEHxyz(Ez, buffer, n, m);
			CDumpEHxyz(Er, buffer, n, m);
			CDumpEHxyz(Hphi, buffer, n, m);
			CDumpEHxyz(Hz, buffer, n, m);
			CDumpEHxyz(Hr, buffer, n, m);
		}
		buffer.append(reinterpret_cast<const char*>(&ds_phi), sizeof(ds_phi));
		buffer.append(reinterpret_cast<const char*>(&ds_z), sizeof(ds_z));
		buffer.append(reinterpret_cast<const char*>(&centerphi), sizeof(centerphi));
		buffer.append(reinterpret_cast<const char*>(&centerz), sizeof(centerz));
		buffer.append(reinterpret_cast<const char*>(&frequency), sizeof(frequency));
		return;
	}

	int CylinderFieldBase::UnSerialize(const char * p, size_t size)
	{
		if (size < coor.Size()) return -1;
		int ret = coor.UnSerialize(p, coor.Size());
		p += coor.Size();
		size -= coor.Size();
		if (ret != 0) return ret;
		size_t n = 0;
		size_t m = 0;
		memcpy(&n, p, sizeof(size_t));
		p += sizeof(size_t);
		size -= sizeof(size_t);
		memcpy(&m, p, sizeof(size_t));
		p += sizeof(size_t);
		size -= sizeof(size_t);
		if (CLoadEHxyz(Ephi, p, n, m, size) != 0) return -1;
		if (CLoadEHxyz(Ez, p, n, m, size) != 0) return -1;
		if (CLoadEHxyz(Er, p, n, m, size) != 0) return -1;

		if (CLoadEHxyz(Hphi, p, n, m, size) != 0) return -1;
		if (CLoadEHxyz(Hz, p, n, m, size) != 0) return -1;
		if (CLoadEHxyz(Hr, p, n, m, size) != 0) return -1;

		if (size < sizeof(ds_phi) * 5) return -1;
		memcpy(&ds_phi, p, sizeof(ds_phi));
		p += sizeof(ds_phi);
		size -= sizeof(ds_phi);
		memcpy(&ds_z, p, sizeof(ds_z));
		p += sizeof(ds_z);
		size -= sizeof(ds_z);
		memcpy(&centerphi, p, sizeof(centerphi));
		p += sizeof(centerphi);
		size -= sizeof(centerphi);
		memcpy(&centerz, p, sizeof(centerz));
		p += sizeof(centerz);
		size -= sizeof(centerz);
		memcpy(&frequency, p, sizeof(frequency));
		p += sizeof(frequency);
		size -= sizeof(frequency);
		return 0;
	}

	int CylinderFieldBase::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
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
