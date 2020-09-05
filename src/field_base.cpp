#include "../include/field_base.h"
#include <fstream>
#include <sstream>
#include <String>
#include <iomanip>
using namespace std;

namespace Common {
	void DumpEHxyz(const vector<vector<complex<double>>>& E, std::string& buffer, size_t n, size_t m) {
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

	int LoadEHxyz(vector<vector<complex<double>>>&E, const char*& p, size_t n, size_t m, size_t& size) {
		bool flag = false;
		if (size < sizeof(size_t)) return -1;
		memcpy(&flag, p, sizeof(flag));
		p += sizeof(bool);
		size -= sizeof(bool);
		if (flag) {
			if (size < n * m * sizeof(double) * 2) return -1;
			FieldBase::Resize(E, n, m);
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
		
	size_t FieldBase::Size() const
	{
		size_t size = coor.Size()
			+ 6 * sizeof(bool)
			+ 2 * sizeof(size_t)
			+ sizeof(ds_x)
			+ sizeof(ds_y)
			+ sizeof(frequency);
		if (Ex.empty()) return size;
		size_t n = Ex.size();
		size_t m = Ex[0].size();
		if (!Ex.empty()) size += n * m * sizeof(double) * 2;
		if (!Ey.empty()) size += n * m * sizeof(double) * 2;
		if (!Ez.empty()) size += n * m * sizeof(double) * 2;
		if (!Hx.empty()) size += n * m * sizeof(double) * 2;
		if (!Hy.empty()) size += n * m * sizeof(double) * 2;
		if (!Hz.empty()) size += n * m * sizeof(double) * 2;

		return size;
	}

	int FieldBase::Open(const std::string & file_name)
	{
		std::ifstream in(file_name);
		if (!in.is_open()) return -1;
		std::string buffer((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		in.close();
		return UnSerialize(buffer);
	}

	int FieldBase::Save(const std::string & file_name) const
	{
		std::ofstream out(file_name);
		if (!out.is_open()) return -1;
		std::string buffer;
		Serialize(buffer);
		out.write(buffer.c_str(), buffer.size());
		out.close();
		return 0;
		
	}

	void FieldBase::Serialize(std::string& buffer) const
	{
		std::cout << "coor.Size():" << coor.Size() << std::endl;
		std::cout << "size:" << Size() << std::endl;
		buffer.reserve(Size());
		coor.Serialize(buffer);
		if (Ex.empty()) {
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
			size_t n = Ex.size();
			size_t m = Ex[0].size();
			buffer.append(reinterpret_cast<const char*>(&n), sizeof(n));
			buffer.append(reinterpret_cast<const char*>(&m), sizeof(m));
			DumpEHxyz(Ex, buffer, n, m);
			DumpEHxyz(Ey, buffer, n, m);
			DumpEHxyz(Ez, buffer, n, m);
			DumpEHxyz(Hx, buffer, n, m);
			DumpEHxyz(Hy, buffer, n, m);
			DumpEHxyz(Hz, buffer, n, m);
		}
		buffer.append(reinterpret_cast<const char*>(&ds_x), sizeof(ds_x));
		buffer.append(reinterpret_cast<const char*>(&ds_y), sizeof(ds_y));
		buffer.append(reinterpret_cast<const char*>(&frequency), sizeof(frequency));
		return;
	}

	int FieldBase::UnSerialize(const char * p, size_t size)
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
		if (LoadEHxyz(Ex, p, n, m, size) != 0) return -1;
		if (LoadEHxyz(Ey, p, n, m, size) != 0) return -1;
		if (LoadEHxyz(Ez, p, n, m, size) != 0) return -1;
		 
		if (LoadEHxyz(Hx, p, n, m, size) != 0) return -1;
		if (LoadEHxyz(Hy, p, n, m, size) != 0) return -1;
		if (LoadEHxyz(Hz, p, n, m, size) != 0) return -1;

		if (size < sizeof(ds_x) * 3) return -1;
		memcpy(&ds_x, p, sizeof(ds_x));
		p += sizeof(ds_x);
		size -= sizeof(ds_x);
		memcpy(&ds_y, p, sizeof(ds_y));
		p += sizeof(ds_y);
		size -= sizeof(ds_y);
		memcpy(&frequency, p, sizeof(frequency));
		p += sizeof(frequency);
		size -= sizeof(frequency);
		return 0;
	}

	int FieldBase::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}

	void FieldBase::Resize(vector<vector<complex<double>>>& E, size_t n, size_t m)
	{
		E.resize(n);
		for (size_t i = 0; i < n; i++) {
			E[i].resize(m);
		}
	}

	int FieldBase::writeFieldBase(const std::string& path) {
		//写文件
		FILE* writefile;
		errno_t err;
		err = fopen_s(&writefile, path.c_str(), "wb");
		if (err != 0) return err;
		double tx, ty, tz, rx, ry, rz, rth;
		int flag;
		tx = coor.pos.x;
		ty = coor.pos.y;
		tz = coor.pos.z;
		rx = coor.rotate_axis.x;
		ry = coor.rotate_axis.y;
		rz = coor.rotate_axis.z;
		rth = coor.rotate_theta;

		if (Ex.empty()) return -1;
		int Nx = Ex.size();
		if (Ex[0].empty()) return -1;
		int Ny = Ex[0].size();


		fwrite(&frequency, sizeof(double), 1, writefile);

		fwrite(&tx, sizeof(double), 1, writefile);
		fwrite(&ty, sizeof(double), 1, writefile);
		fwrite(&tz, sizeof(double), 1, writefile);

		fwrite(&rx, sizeof(double), 1, writefile);
		fwrite(&ry, sizeof(double), 1, writefile);
		fwrite(&rz, sizeof(double), 1, writefile);

		fwrite(&rth, sizeof(double), 1, writefile);

		fwrite(&Nx, sizeof(int), 1, writefile);
		fwrite(&Ny, sizeof(int), 1, writefile);

		fwrite(&ds_x, sizeof(double), 1, writefile);
		fwrite(&ds_y, sizeof(double), 1, writefile);

		if (Hx.empty()) flag = 0;
		else flag = 1;

		fwrite(&flag, sizeof(int), 1, writefile);

		if (flag == 0) {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Ex[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Ey[i][j], sizeof(complex<double>), 1, writefile);
				}
		}
		else {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Ex[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Ey[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Ez[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Hx[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Hy[i][j], sizeof(complex<double>), 1, writefile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fwrite(&Hz[i][j], sizeof(complex<double>), 1, writefile);
				}
		}
		fclose(writefile);
		return err;
	}
	int FieldBase::readFieldBase(const std::string& path)
	{
		FILE* readfile = NULL;
		errno_t err;
		err = fopen_s(&readfile, path.c_str(), "rb");
		if (err != 0) return err;
		double tx, ty, tz, rx, ry, rz, rth;
		double freq;
		int Nx, Ny;
		double dx = 0.0;	double dy = 0.0;
		fread(&freq, sizeof(double), 1, readfile);

		fread(&tx, sizeof(double), 1, readfile);
		fread(&ty, sizeof(double), 1, readfile);
		fread(&tz, sizeof(double), 1, readfile);

		fread(&rx, sizeof(double), 1, readfile);
		fread(&ry, sizeof(double), 1, readfile);
		fread(&rz, sizeof(double), 1, readfile);

		fread(&rth, sizeof(double), 1, readfile);

		fread(&Nx, sizeof(int), 1, readfile);
		fread(&Ny, sizeof(int), 1, readfile);

		fread(&dx, sizeof(double), 1, readfile);
		fread(&dy, sizeof(double), 1, readfile);

		coor.pos = Vector3(tx, ty, tz);
		coor.rotate_axis = Vector3(rx, ry, rz);
		coor.rotate_theta = rth;
		ds_x = dx;		ds_y = dy;
		frequency = freq;
		int flag;
		fread(&flag, sizeof(int), 1, readfile);
		if (flag == 0) {
			Ex.resize(Nx);		Ey.resize(Nx);
			for (int i = 0; i < Nx; i++) {
				Ex[i].resize(Ny);		Ey[i].resize(Ny);
			}
		}
		else {
			Ex.resize(Nx);		Ey.resize(Nx);		Ez.resize(Nx);
			Hx.resize(Nx);		Hy.resize(Nx);		Hz.resize(Nx);
			for (int i = 0; i < Nx; i++) {
				Ex[i].resize(Ny);		Ey[i].resize(Ny);		Ez[i].resize(Ny);
				Hx[i].resize(Ny);		Hy[i].resize(Ny);		Hz[i].resize(Ny);
			}
		}

		//下面是读取环节
		if (flag == 0) {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j<Ny; j++) {
					fread(&Ex[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j<Ny; j++) {
					fread(&Ey[i][j], sizeof(complex<double>), 1, readfile);
				}
		}
		else {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Ex[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Ey[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Ez[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Hx[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Hy[i][j], sizeof(complex<double>), 1, readfile);
				}
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					fread(&Hz[i][j], sizeof(complex<double>), 1, readfile);
				}
		}
		fclose(readfile);
		return  err;
	}

	int FieldBase::writeFieldBaseASCII(const std::string& path) {
		//写文件
		errno_t err = 0;
		double rad2deg = 180.0/ 3.1415926535898;
		if (Ex.empty()) return -1;//如果里面没有数据就返回
		int Nx, Ny;
		Nx = Ex.size(); Ny = Ex[0].size();
		int flag = 0;	if (!Hx.empty()) flag = 1;
		ofstream outfile(path);
		stringstream temp;
		temp << scientific << setprecision(6) << coor.pos.x << " " << coor.pos.y << " " << coor.pos.z << " "
			<< coor.rotate_axis.x << " " << coor.rotate_axis.y << " "<< coor.rotate_axis.z << " " << coor.rotate_theta << " "
			<< Nx << " " << Ny << " " << ds_x << " " << ds_y << " " << flag << " " << frequency << endl;
		if (flag == 0) {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++) {
					temp
						<< abs(Ex[i][j]) << " " << arg(Ex[i][j]) * rad2deg << " "
						<< abs(Ey[i][j]) << " " << arg(Ey[i][j]) * rad2deg << " " << endl;
				}
		}
		else if (flag == 1) {
			for (int i = 0; i < Nx; i++)
				for (int j = 0; j < Ny; j++){
					temp
						<< abs(Ex[i][j]) << " " << arg(Ex[i][j]) * rad2deg << " "
						<< abs(Ey[i][j]) << " " << arg(Ey[i][j]) * rad2deg << " "
						<< abs(Ez[i][j]) << " " << arg(Ez[i][j]) * rad2deg << " "
						<< abs(Hx[i][j]) << " " << arg(Hx[i][j]) * rad2deg << " "
						<< abs(Hy[i][j]) << " " << arg(Hy[i][j]) * rad2deg << " "
						<< abs(Hz[i][j]) << " " << arg(Hz[i][j]) * rad2deg << " " << endl;
				}
		}

		outfile << temp.str() << endl;
		outfile.close();
		return err;
	}
	int FieldBase::readFieldBaseASCII(const std::string& path)
	{
		const double Pi = 3.1415926535898;
		errno_t err = 0;
		ifstream file;
		string temp;
		file.open(path);
		if (!file.is_open()) return -1;
		stringstream tempfile;
		tempfile << file.rdbuf();
		getline(tempfile, temp);
		istringstream tempLine(temp);

		double tx, ty, tz, rx, ry, rz, rth;
		tempLine >> tx >> ty >> tz >> rx >> ry >> rz >> rth;

		coor.pos = Vector3(tx, ty, tz);
		coor.SetRotate(Vector3(rx, ry, rz), rth);
		
		int Nx;	int Ny;
		tempLine >> Nx >> Ny >> ds_x >> ds_y;
		int flag; double freq;
		tempLine >> flag >> freq;
		frequency = freq;

		Ex.resize(Nx);	Ey.resize(Nx);
		for (int i = 0; i < Nx; ++i)
		{
			Ex[i].resize(Ny);	Ey[i].resize(Ny);
		}

		if (flag == 1) // 不是源才分配其他的场分量
		{
			Ez.resize(Nx);	Hx.resize(Nx);	Hy.resize(Nx);	Hz.resize(Nx);
			for (int i = 0; i < Nx; ++i)
			{
				Ez[i].resize(Ny);	Hx[i].resize(Ny);	Hy[i].resize(Ny);	Hz[i].resize(Ny);
			}
		}

		double deg2rad = Pi / 180.0;

		if (flag == 0) {
			for (int i = 0; i < Nx; i++) {
				for (int j = 0; j < Ny; j++) {

					getline(tempfile, temp);
					istringstream tempLine(temp);

					double a1, p1, a2, p2;		
					tempLine >> a1 >> p1 >> a2 >> p2;

					Ex[i][j] = complex<double>(a1*cos(p1 * deg2rad), a1*sin(p1 * deg2rad));
					Ey[i][j] = complex<double>(a2*cos(p2 * deg2rad), a2*sin(p2 * deg2rad));
				}
			}
		}
		else if(flag == 1) {
			for (int i = 0; i < Nx; i++){
				for (int j = 0; j < Ny; j++){

					getline(tempfile, temp);
					istringstream tempLine(temp);

					double a1, p1, a2, p2, a3, p3;		double a4, p4, a5, p5, a6, p6;
					tempLine >> a1 >> p1 >> a2 >> p2 >> a3 >> p3 >> a4 >> p4 >> a5 >> p5 >> a6 >> p6;

					Ex[i][j] = complex<double>(a1*cos(p1 * deg2rad),	a1*sin(p1 * deg2rad));
					Ey[i][j] = complex<double>(a2*cos(p2 * deg2rad),	a2*sin(p2 * deg2rad));
					Ez[i][j] = complex<double>(a3*cos(p3 * deg2rad),	a3*sin(p3 * deg2rad));
					Hx[i][j] = complex<double>(a4*cos(p4 * deg2rad),	a4*sin(p4 * deg2rad));
					Hy[i][j] = complex<double>(a5*cos(p5 * deg2rad),	a5*sin(p5 * deg2rad));
					Hz[i][j] = complex<double>(a6*cos(p6 * deg2rad),	a6*sin(p6 * deg2rad));

				}
			}
		}

		file.close();
		return  err;
	}
}
