#pragma once
#include <memory>
#include <vector>
#include <glad/glad.h>

namespace OpenGL {
	struct RenderCommandMDI;
	struct MDIBatch;
	class BufferManager;
}
namespace ECS    { class World;        }
namespace EDITOR { struct EditorState; }

namespace OpenGL
{
	class Renderer
	{
	public:
		Renderer(BufferManager* buffManager, ECS::World* w) : bufferManager(buffManager), world(w) {}

		void Render();

	private:
		BufferManager* bufferManager = nullptr;
		ECS::World* world = nullptr;
	};
}
