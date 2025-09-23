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
		[[nodiscard]] EditorRenderState          &GetEditorRenderState()  { return editorRenderState; };
		[[nodiscard]] GameRenderState            &GetGameRenderState()    { return gameRenderState;   };

		[[nodiscard]] const RenderState          *GetRenderState()       const { return renderState;       };
		[[nodiscard]] const EditorRenderState    &GetEditorRenderState() const { return editorRenderState; };
		[[nodiscard]] const GameRenderState      &GetGameRenderState()   const { return gameRenderState;   };

	private:
		RenderState* renderState{};
		EditorRenderState editorRenderState{};
		GameRenderState gameRenderState{};
	};
}
