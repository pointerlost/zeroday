#pragma once
#include <iostream>

namespace Zeroday
{
	struct Texture {
		std::string name;
		std::string path;
		uint32_t glID = 0;
		uint64_t bindlessHandle = 0;
		bool     resident = false;
	};

}
