//
// Created by pointerlost on 8/19/25.
//
#pragma once
#include <glad/glad.h>
#include "Graphics/OpenGL/GPU_buffers.h"

inline constexpr uint32_t INVALID_ENTITY = entt::null;

inline constexpr int MAX_ENTITIES = 65536;

inline constexpr int DIRECTIONAL_LIGHT_TYPE = 0;
inline constexpr int POINT_LIGHT_TYPE       = 1;
inline constexpr int SPOT_LIGHT_TYPE        = 2;

// SSBO binding locations
constexpr int BINDING_PHASE_INPUT      = 0; // For CPU->GPU data
constexpr int BINDING_PHASE_OUTPUT     = 1; // For GPU->CPU data
constexpr int BINDING_PHASE_METADATA   = 2; // For metadata (renderCommandMDI)
constexpr int BINDING_PHASE_SCRATCH    = 3; // For temporary data
constexpr int BINDING_ENTITY_METADATA  = 4;
constexpr int BINDING_VISIBLE_ENTITIES = 5;
constexpr int BINDING_COMMANDS         = 6;
constexpr int BINDING_PAYLOADS         = 7;