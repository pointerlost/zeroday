#pragma once
namespace ecs { class World; }

namespace opengl
{
	class Renderer
	{
	public:
		explicit Renderer(ecs::World* w) : world(w) {}

		void Render();

	private:
		ecs::World* world = nullptr;
	};
}
