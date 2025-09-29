#pragma once
#include <memory>
#include "GPURenderer.h"
#include "Core/Base.h"

namespace Zeroday::opengl {

	class Renderer3D
	{
	public:
		explicit Renderer3D(Scene* scene)
			: m_Scene(scene), m_GPURenderer(CreateScope<GPURenderer>(scene)) {}

		void Init()     { m_GPURenderer->Init();     }
		void Render()   { m_GPURenderer->Render();   }
		void Shutdown() { m_GPURenderer->Shutdown(); }

	private:
		Scene* m_Scene = nullptr;
		std::unique_ptr<GPURenderer> m_GPURenderer = nullptr;
	};

}
