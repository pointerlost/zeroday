#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Zeroday {

	class File {
	public:
		static File& get();

		std::string readFromFile(const std::string& name);

		[[nodiscard]] bool exists(const std::string& path) const noexcept;
	
		File() = default; // you can't create this object from outside
		File(const File&) = delete;
		void operator=(const File&) = delete;
	};
}