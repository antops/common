#pragma once
#include <vector>
#include "coordinate.h"

namespace Common {
	struct RestrictionParam {
		std::vector<double> param;
		int type;
		Coordinate coor;

		std::string Debug() const {
			return "coor:" + coor.Debug() + ","
				+ "type:" + std::to_string(type) + ","
				+ "param size:" + std::to_string(param.size());
		}

		std::string Serialize() const;
		void UnSerialize(const char* p, size_t size);
		void UnSerialize(const std::string& buffer);
	};
	struct ComponentParam {
		std::string name;
		std::vector<double> param;
		std::string param_str;
		Coordinate coor;
		std::vector<RestrictionParam> restriction;

		std::string Debug() const {
			return "coor:" + coor.Debug() + ","
				+ "name:" + name + ","
				+ "name:" + param_str + ","
				+ "param size:" + std::to_string(param.size())
				+ "restriction size:" + std::to_string(restriction.size());
		}

		std::string Serialize() const;
		void UnSerialize(const char* p, size_t size);
		void UnSerialize(const std::string& buffer);

	};
}