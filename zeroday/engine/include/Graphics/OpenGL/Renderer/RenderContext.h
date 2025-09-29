#pragma once
#include "RenderState.h"

namespace Zeroday::opengl
{
	class RenderContext
	{
	public:
		RenderContext()  = default;
		~RenderContext() = default;

		[[nodiscard]] RenderState                *GetRenderState()        { return renderState;       };
		[[nodiscard]] const RenderState          *GetRenderState()       const { return renderState;       };

	private:
		RenderState* renderState{};
	};
}
