#pragma once
#include <vector>
#include "coordinate.h"

namespace Common {
	enum ComponentType
	{
		Analysis,
		STL,
	};
	struct RestrictionParam {
		std::vector<double> param;
		int type;
		Coordinate coor;

		std::string Debug() const {
			return "coor:" + coor.Debug() + ","
				+ "type:" + std::to_string(type) + ","
				+ "param size:" + std::to_string(param.size());
		}

		void Serialize(std::string& buffer) const;
		int UnSerialize(const char* p, size_t size);
		int UnSerialize(const std::string& buffer);
	};

	struct ComponentParam {
		std::string name;
		std::vector<double> param;
		std::string param_str;
		Coordinate coor;
		ComponentType type;
		std::vector<RestrictionParam> restriction;
		void* data_ptr; // 用于保存stl指针

		std::string Debug() const {
			return "coor:" + coor.Debug() + ","
				+ "type:" + std::to_string(type) + ","
				+ "name:" + name + ","
				+ "name:" + param_str + ","
				+ "param size:" + std::to_string(param.size())
				+ "restriction size:" + std::to_string(restriction.size());
		}

		void Serialize(std::string& buffer) const;
		int UnSerialize(const char* p, size_t size);
		int UnSerialize(const std::string& buffer);
	};
}