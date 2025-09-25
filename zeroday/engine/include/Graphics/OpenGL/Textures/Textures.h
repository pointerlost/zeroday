#pragma once
#include <cstdint>

namespace Zeroday
{
	struct Texture {
		std::string m_Name;
		std::string m_Path;
		uint32_t m_GLId = 0;
		uint64_t m_BindlessHandle = 0;
		bool     m_Resident = false;
	};

}
