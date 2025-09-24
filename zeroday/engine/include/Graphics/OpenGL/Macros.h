//
// Created by pointerlost on 8/19/25.
//
#pragma once
#include <glad/glad.h>
#include "entt/entt.hpp"

inline constexpr uint32_t INVALID_ENTITY = entt::null;

inline constexpr int MAX_ENTITIES = 262144; // do it power of two, easy for allocation
inline constexpr int MAX_LIGHTS = 4096;

inline constexpr int DIRECTIONAL_LIGHT_TYPE = 0;
inline constexpr int POINT_LIGHT_TYPE       = 1;
inline constexpr int SPOT_LIGHT_TYPE        = 2;

// SSBO binding locations
