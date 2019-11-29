#include "component_param.h"

namespace Common {
	std::string RestrictionParam::Serialize() const
	{
		std::string buffer = coor.Serialize();
		buffer.append(reinterpret_cast<const char*>(&type), sizeof(type));
		int size = param.size();
		buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
		for (auto x : param) {
			buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
		}
		return buffer;
	}
	void RestrictionParam::UnSerialize(const char * p, size_t size)
	{
		if (size < sizeof(type) + sizeof(int) + coor.Size()) return;
		coor.UnSerialize(p, coor.Size());
		p += coor.Size();
		memcpy(&type, p, sizeof(type));
		p += sizeof(type);
		int num = 0;
		memcpy(&num, p, sizeof(num));
		p += sizeof(num);
		if (num <= 0 || num + sizeof(type) + sizeof(int) + coor.Size() != size) return;
		param.resize(num);
		for (size_t i = 0; i < num; i++) {
			memcpy(&param[i], p, sizeof(double));
			p += sizeof(double);
		}
	}
	void RestrictionParam::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}

	std::string ComponentParam::Serialize() const
	{
		std::string buffer = coor.Serialize();
		int size = name.size();
		buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
		buffer.append(name);
		size = param_str.size();
		buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
		buffer.append(param_str);
		size = param.size();
		buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
		for (auto x : param) {
			buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
		}
		size = restriction.size();
		buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
		for (const auto& x : restriction) {
			std::string tmp = x.Serialize();
			size = tmp.size();
			buffer.append(reinterpret_cast<const char*>(&size), sizeof(size));
			buffer.append(tmp);
		}
		return buffer;
	}
	void ComponentParam::UnSerialize(const char * p, size_t size)
	{
		if (size < sizeof(int) + coor.Size()) return;
		coor.UnSerialize(p, coor.Size());
		p += coor.Size();
		size -= coor.Size();
		int num = 0;
		memcpy(&num, p, sizeof(num));
		p += sizeof(int);
		size -= sizeof(int);

		if (size < sizeof(int) + num) return;
		name.assign(p, num);
		p += num;
		size -= num;
		memcpy(&num, p, sizeof(num));
		p += sizeof(int);
		size -= sizeof(int);

		if (size < sizeof(int) + num) return;
		param_str.assign(p, num);
		p += num;
		size -= num;
		memcpy(&num, p, sizeof(num));
		p += sizeof(int);
		size -= sizeof(int);

		if (size < num) return;
		param.resize(num);
		for (size_t i = 0; i < num; i++) {
			memcpy(&param[i], p, sizeof(double));
			p += sizeof(double);
			size -= sizeof(double);
		}

		memcpy(&num, p, sizeof(num));
		p += sizeof(int);
		size -= sizeof(int);

		restriction.resize(num);
		for (size_t i = 0; i < num; i++) {
			int len = 0;
			if (size < sizeof(int)) return;
			memcpy(&len, p, sizeof(int));
			p += sizeof(int);
			size -= sizeof(int);
			if (size < len) return;
			restriction[i].UnSerialize(p, len);
			p += len;
			size -= len;
		}
	}
	void ComponentParam::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}
}
