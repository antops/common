#include "../include/field_base.h"
#include <fstream>

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
		for (size_t i = 0; i < m; i++) {
			E[i].resize(m);
		}
	}
}
