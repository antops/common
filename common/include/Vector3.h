/*
*	created by liyun 2017/10/20
*   function 定义了三维向量，重载了-，定义了点乘和叉乘
*   version 1.0
*/

#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

namespace Common {

	class Vector3
	{
	public:
		Vector3(double x = 0, double y = 0, double z = 0)
			:x(x), y(y), z(z)
		{

		};

		~Vector3()
		{
		};

		// 重载"-"
		Vector3 operator - (const Vector3 &v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		};

		// 重载"+"
		Vector3 operator + (const Vector3 &v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		};

		// 标量乘
		Vector3 operator * (const double &v) const
		{
			return Vector3(x * v, y * v, z * v);
		};

		// 点乘
		double Dot(const Vector3 &v) const
		{
			return x*v.x + y*v.y + z*v.z;
		};

		// 叉乘
		Vector3 Cross(const Vector3 &v) const
		{
			return Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
		}

		// 归一化
		void Normalization()
		{
			double temp = pow((x*x + y*y + z*z), 0.5);
			if (abs(temp) < 1e-8)
				return;
			x /= temp;
			y /= temp;
			z /= temp;
		}

		// 计算长度
		double Length() const
		{
			return pow((x*x + y*y + z*z), 0.5);
		}

		double Area() const
		{
			return x*x + y*y + z*z;
		}

		void Set(double x1, double y1, double z1)
		{
			x = x1; y = y1; z = z1;
		}

		std::string Debug() const {
			return "(" + std::to_string(x) + ","
				+ std::to_string(y) + ","
				+ std::to_string(z) + ")";
		}

		void Save(std::ofstream & savefile) const {
			savefile << x << " "
				<< y << " "
				<< z << " ";
		}

		void Open(std::ifstream & readfile) {
			readfile >> x >> y >> z;
		}

		std::string Serialize(bool is_double = true) const {
			std::string buffer;
			if (is_double) {
				buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
				buffer.append(reinterpret_cast<const char*>(&y), sizeof(y));
				buffer.append(reinterpret_cast<const char*>(&z), sizeof(z));
			}
			else {
				float x1 = x;
				float y1 = y;
				float z1 = z;

				buffer.append(reinterpret_cast<const char*>(&x1), sizeof(x1));
				buffer.append(reinterpret_cast<const char*>(&y1), sizeof(y1));
				buffer.append(reinterpret_cast<const char*>(&z1), sizeof(z1));
			}
			return buffer;
		}

		void UnSerialize(const char* p, size_t size) {
			if (size == sizeof(double) * 3) {
				memcpy(&x, p, sizeof(double));
				p += sizeof(double);
				memcpy(&y, p, sizeof(double));
				p += sizeof(double);
				memcpy(&z, p, sizeof(double));
			}
			else if (size == sizeof(float) * 3) {
				float x1 = 0.0;
				float y1 = 0.0;
				float z1 = 0.0;
				memcpy(&x1, p, sizeof(float));
				p += sizeof(float);
				memcpy(&y1, p, sizeof(float));
				p += sizeof(float);
				memcpy(&z1, p, sizeof(float));
				x = x1;
				y = y1;
				z = z1;
			}
			return;
		}

		void UnSerialize(const std::string& buffer) {
			const char* p = buffer.c_str();
			return UnSerialize(p, buffer.size());
		}

		double x, y, z;
		
	};

}

#endif // VECTOR3_H
