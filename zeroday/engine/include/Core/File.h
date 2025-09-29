#pragma once
#include <string>

namespace Zeroday {

	class File {
	public:
		static File& Get();

		std::string ReadFromFile(const std::string& name);

		[[nodiscard]] bool Exists(const std::string& path) const noexcept;
	
		File() = default;
		File(const File&) = delete;
		void operator=(const File&) = delete;
	};
}