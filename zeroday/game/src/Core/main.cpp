#include "Core/main.h"
#include <memory>

int main() {
	const auto engine = Zeroday::CreateScope<Zeroday::Engine>();

	try {
		// init engine pointers
		engine->InitPointerObjects();

		// init resources
		if (!engine->InitResources()) {
			Zeroday::Error("Failed to initialize resources!");
			return -1;
		}

		// start the engine
		if (!engine->Run()) {
			Zeroday::Error("Engine run failed or stopped because of some reasons!");
			return -1;
		}
	} catch (const std::exception& e) {
		Zeroday::Error("Engine source loading encountered a problem! Error: " + std::string(e.what()));
		return -1;
	}

	return 0;
}