#pragma once
#include <iostream>


namespace Zeroday {

	inline void Warn(const std::string& msg) {
        std::cerr << "[WARN] " << msg << "\n";
	}

	inline void Error(const std::string& msg) {
        std::cerr << "[ERROR] " << msg << "\n";
	}

	inline void Info(const std::string& msg) {
        std::cerr << "[INFO] " << msg << "\n";
	}

}

