#pragma once
#include "Vector3.h"
#include <string>
namespace Common {
	struct Coordinate {
		Vector3 U;
		Vector3 V;
		Vector3 N;

		Vector3 pos;
		double scale = 1.0;

		double rotate_theta = 0.0;
		Vector3 rotate_axis;

		std::string Debug() const {
			return "U" + U.Debug() + ","
				+ "V" + V.Debug() + ","
				+ "N" + N.Debug() + ","
				+ "pos" + pos.Debug() + ","
				+ "scale" + std::to_string(scale) + ","
				+ "rotate_theta" + std::to_string(rotate_theta) + ","
				+ "rotate_axis" + rotate_axis.Debug();
		}

		size_t Size() const {
			return U.Size() + V.Size() + N.Size()
				+ pos.Size() + rotate_axis.Size()
				+ sizeof(double) * 2;
		}

		void Save(std::ofstream & savefile) const;

		void Open(std::ifstream & readfile);

		void Serialize(std::string& buffer) const;
		int UnSerialize(const char* p, size_t size);
		int UnSerialize(const std::string& buffer);

		bool SetUV(const Vector3& u, const Vector3& v);
		void SetRotate(const Vector3& axis, double theta);
	};
}