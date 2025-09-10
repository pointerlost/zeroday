cmake_minimum_required(VERSION 3.20)
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Vcpkg integration
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

add_library(glad_local STATIC
        third_party/glad/src/glad.c
)

# Include directories for local GLAD
target_include_directories(glad_local PUBLIC
        third_party/glad/include
)

add_library(engine STATIC
        src/core/Engine.cpp
        src/core/Window.cpp
        src/core/File.cpp
        src/core/AssetManager.cpp
        src/ECS/World.cpp
        src/ECS/ComponentStorage.cpp
        src/ECS/Components/TransformComponent.cpp
        src/ECS/Components/MaterialComponent.cpp
        src/ECS/Components/CameraComponent.cpp
        src/Math/Math.cpp
        src/Graphics/OpenGL/Shader.cpp
        src/Graphics/OpenGL/Mesh/MeshLibrary.cpp
        src/Graphics/OpenGL/Model/Model.cpp
        src/Graphics/OpenGL/Model/ModelLoader.cpp
        src/Graphics/OpenGL/Renderer/RenderContext.cpp
        src/Graphics/OpenGL/Renderer/Renderer.cpp
        src/Graphics/OpenGL/Renderer/RenderState.cpp
        src/Graphics/OpenGL/Lighting/Light.cpp
        src/Graphics/OpenGL/Material/material_lib.cpp
        src/Graphics/OpenGL/Textures/TextureManager.cpp
        src/Graphics/OpenGL/Textures/stb_image.cpp
        src/Input/Input.cpp
        src/Input/InputContext.cpp
        src/Input/InputComponent.cpp
        src/Input/InputComponentFactory.cpp
        src/CallBack/CallBack.cpp
        src/Graphics/OpenGL/Mesh/MeshFactory.cpp
        src/Graphics/OpenGL/Mesh/MeshData3D.cpp
        src/ECS/SceneObjectFactory.cpp
        src/Graphics/OpenGL/Transformations/Transformations.cpp
        src/Editor/Editor.cpp
        src/Editor/Panel/EditorPanel.cpp
        src/Editor/Panel/SceneHierarchyPanel.cpp
        src/Editor/Panel/InspectorPanel.cpp
        src/Editor/Panel/MenuBarPanel.cpp
        src/ImGui/ImGuiLayer.cpp
        src/ImGui/ImGuiScoped.cpp
        src/ImGui/ImGuiObjectState.cpp
        src/ImGui/TransformGizmo.cpp
        src/Graphics/OpenGL/BufferManager.cpp
        third_party/ImGuizmo/ImGuizmo.cpp

        # Header-only files (must be explicitly listed for IDE support)
        include/ECS/Entity.h
        include/ECS/ComponentStorage.h
        include/ECS/World.h
        include/ECS/Components/MeshComponent.h
        include/ECS/Components/TransformComponent.h
        include/ECS/Components/LightComponent.h
        include/ECS/Components/MaterialComponent.h
        include/ECS/Components/NameComponent.h
        include/ECS/Components/ModelComponent.h
        include/ECS/Components/CameraComponent.h
        include/Graphics/OpenGL/Mesh/MeshLibrary.h
        include/core/AssetManager.h
        include/Graphics/OpenGL/Shader.h
        include/Editor/Editor.h
        include/Editor/Panel/EditorPanel.h
        include/Editor/State/EditorState.h
        include/Editor/Panel/SceneHierarchyPanel.h
        include/Editor/Panel/InspectorPanel.h
        include/Editor/Panel/MenuBarPanel.h
        include/Graphics/OpenGL/Model/Model.h
        include/Graphics/OpenGL/Model/ModelLoader.h
        include/Graphics/OpenGL/ssbo_types.h
        include/Graphics/OpenGL/Macros.h
        include/Graphics/OpenGL/Renderer/RenderCommand.h
        include/Graphics/OpenGL/Renderer/RenderState.h
        include/Graphics/OpenGL/Material/material.h
        include/ECS/freelist_registry.h
        include/ECS/component_registry.h
        include/Graphics/OpenGL/Renderer/RenderContext.h
        include/Graphics/OpenGL/Renderer/Renderer.h
        include/Graphics/OpenGL/Lighting/Light.h
        include/Scene/SceneObjectFactory.h
        include/Graphics/OpenGL/Textures/TextureManager.h
        include/Graphics/OpenGL/Material/material_lib.h
        include/Graphics/OpenGL/Textures/Textures.h
        include/Graphics/OpenGL/Textures/stb_image.h
        include/ImGui/ImGuiLayer.h
        include/ImGui/ImGuiScoped.h
        include/ImGui/ImGuiObjectState.h
        include/ImGui/TransformGizmo.h
        include/Graphics/OpenGL/Transformations/Transformations.h
        include/Math/Math.h
        include/Math/RayMath.h
        include/core/Engine.h
        include/core/Window.h
        include/core/File.h
        include/core/Logger.h
        include/Input/Input.h
        include/Input/InputContext.h
        include/Input/InputComponent.h
        include/Input/InputComponentFactory.h
        include/CallBack/CallBack.h
        include/Graphics/OpenGL/Mesh/MeshFactory.h
        include/Graphics/OpenGL/Mesh/MeshData3D.h
        include/Graphics/OpenGL/BufferManager.h
        src/Utility/buffer_updater.cpp
        include/Utility/buffer_updater.h
        include/core/Services.h
        src/core/Services.cpp
        src/ECS/Entity.cpp
        src/ECS/component_registry.cpp
        include/ECS/EntityManager.h
        src/ECS/EntityManager.cpp
)

# Include directories
target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

# Find vcpkg packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Link vcpkg dependencies
target_link_libraries(engine PUBLIC
        glad_local
        glfw
        glm::glm
        nlohmann_json::nlohmann_json
        imgui::imgui
        assimp::assimp
)

# Copy assets and shaders to build directory
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "Copying assets and shaders to build directory"
)

# Configure engine config files
set(SHADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(COMPUTE_SHADERS_DIR "${SHADERS_DIR}/computeOpenGL")
set(ASSETS_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
set(TEXTURES_DIR "${ASSETS_DIR}/textures")
set(MODEL_DIR "${ASSETS_DIR}/models")
set(SHADERS_CONFIG_PATH "${SHADERS_DIR}/config/shaders.json")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/core/Config.h
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/core/EngineConfig.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/core/EngineConfig.h
)

# Compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic -O2)
elseif(MSVC)
    target_compile_options(engine PRIVATE /W4 /O2)
endif()

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")