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

		void InitEditorState()     { m_GPURenderer->InitEditorState();     }
		void RenderEditorState()   { m_GPURenderer->RenderEditorState();   }
		void ShutdownEditorState() { m_GPURenderer->ShutdownEditorState(); }

	private:
		Scene* m_Scene = nullptr;
		std::unique_ptr<GPURenderer> m_GPURenderer = nullptr;
	};

}
