#pragma once
#include "Vector3.h"
#include <string>
#include <vector>
namespace Common {
	struct RayLineCluster
	{
		std::vector<Vector3> start_point;  // ���
		std::vector<Vector3> normal_line;  // ����
		std::string Serialize() const;
		void UnSerialize(const char* p, size_t size);
		void UnSerialize(const std::string& buffer);
	};
}