#include "core/main.h"
#include "core/Logger.h"

int main()
{
	const auto engine = CreateScope<Zeroday::Engine>();

	try {
		// init engine pointers
		engine->InitPointerObjects();

		// init resources
		if (!engine->InitResources()) {
			Logger::Error("Failed to initialize resources!");
			return -1;
		}

		// start the engine
		if (!engine->Run()) {
			Logger::Error("Engine run failed or stopped because of some reasons!");
			return -1;
		}
	} catch (const std::exception& e) {
		Logger::Error("Engine source loading encountered a problem! Error: " + std::string(e.what()));
		return -1;
	}

	return 0;
}