#pragma once
#include <iostream>

/*
* LOGGER FUNCTIONS PURPOSES:

* Error: Critical error, a condition that prevents the program from working properly
(example: A significant part of the program is affected, inoperable)

--- 

* Warn: Issues that need attention but do not stop the program flow
(example: Unexpected situation but the system can continue)

---

* Info: For informational purposes, logs in normal flow
(example: successful operations)

*/

namespace Logger
{
	inline void warn(const std::string& msg);
	inline void error(const std::string& msg);
	inline void info(const std::string& msg);
}


// Declarations
inline void Logger::info(const std::string& msg) {
	std::cerr << "[INFO] " << msg << "\n";
}
inline void Logger::error(const std::string& msg) {
	std::cerr << "[ERROR] " << msg << "\n";
}
inline void Logger::warn(const std::string& msg) {
	std::cerr << "[WARN] " << msg << "\n";
}
