#include "component_param.h"

namespace Common {
	void RestrictionParam::Serialize(std::string& buffer) const
	{
		return;
	}

	int RestrictionParam::UnSerialize(const char * p, size_t size)
	{
		return 0;
	}

	int RestrictionParam::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}

	void ComponentParam::Serialize(std::string& buffer) const
	{
		return;
	}

	int ComponentParam::UnSerialize(const char * p, size_t size)
	{
		return 0;
	}

	int ComponentParam::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}
}
