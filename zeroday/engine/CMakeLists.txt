if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "gcc compiler not found. Please install it with: sudo apt install gcc (for linux)")
    message(FATAL_ERROR "you should install gcc-13+ version!")
endif()

if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "g++ compiler not found. Please install it with: sudo apt install g++ (for linux)")
    message(FATAL_ERROR "you should install g++-13+ version!")
endif()

# Vcpkg integration
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

add_library(glad_local STATIC
    vendor/glad/src/glad.c
)

# Include directories for local GLAD
target_include_directories(glad_local PUBLIC
    vendor/glad/include
)

add_library(ZeroDayEngine STATIC
    src/Core/Engine.cpp
    src/Core/Window.cpp
    src/Core/File.cpp
    src/Core/AssetManager.cpp
    src/Math/Math.cpp
    src/Graphics/OpenGL/Shader.cpp
    src/Graphics/OpenGL/Mesh/MeshLibrary.cpp
    src/Graphics/OpenGL/Model/Model.cpp
    src/Graphics/OpenGL/Model/ModelLoader.cpp
    src/Graphics/OpenGL/Renderer/RenderContext.cpp
    src/Graphics/OpenGL/Renderer/Renderer3D.cpp
    src/Graphics/OpenGL/Renderer/RenderState.cpp
    src/Graphics/OpenGL/Lighting/Light.cpp
    src/Graphics/OpenGL/Textures/stb_image.cpp
    src/Input/Input.cpp
    src/Input/InputContext.cpp
        src/CallBack/CallBack.cpp
    src/Graphics/OpenGL/Mesh/MeshFactory.cpp
    src/Graphics/OpenGL/Mesh/MeshData3D.cpp
    src/Scene/SceneObjectFactory.cpp
    src/Graphics/OpenGL/Transformations/Transformations.cpp
    src/Editor/Editor.cpp
    src/Editor/EditorPanel.cpp
    src/Editor/SceneHierarchyPanel.cpp
    src/Editor/InspectorPanel.cpp
    src/Editor/MenuBarPanel.cpp
    src/ImGui/ImGuiLayer.cpp
    src/ImGui/ImGuiScoped.cpp
    src/ImGui/ImGuiObjectState.cpp
    src/ImGui/TransformGizmo.cpp
        src/Graphics/OpenGL/Renderer/GPURenderer.cpp
    src/Graphics/OpenGL/Camera/Camera.cpp
    src/Core/Services.cpp
    src/Core/Logger.cpp
    src/Scene/Scene.cpp
    src/Core/UUID.cpp
    src/Scene/Entity.cpp
    src/Graphics/OpenGL/Renderer/SceneRenderer.cpp
    src/Graphics/OpenGL/Material/material.cpp
    src/Scene/Components.cpp
    src/Graphics/OpenGL/Textures/Textures.cpp
    vendor/ImGuizmo/ImGuizmo.cpp

    # Header-only files (must be explicitly listed for IDE support)
    include/Graphics/OpenGL/Mesh/MeshLibrary.h
    include/Core/AssetManager.h
    include/Graphics/OpenGL/Shader.h
    include/Editor/Editor.h
    include/Editor/EditorPanel.h
    include/Editor/EditorState.h
    include/Editor/SceneHierarchyPanel.h
    include/Editor/InspectorPanel.h
    include/Editor/MenuBarPanel.h
    include/Graphics/OpenGL/Model/Model.h
    include/Graphics/OpenGL/Model/ModelLoader.h
    include/Graphics/OpenGL/GPU_buffers.h
    include/Graphics/OpenGL/Macros.h
    include/Graphics/OpenGL/Renderer/RenderCommand.h
    include/Graphics/OpenGL/Renderer/RenderState.h
    include/Graphics/OpenGL/Material/material.h
    include/Graphics/OpenGL/Renderer/RenderContext.h
    include/Graphics/OpenGL/Renderer/Renderer3D.h
    include/Graphics/OpenGL/Lighting/Light.h
    include/Scene/SceneObjectFactory.h
    include/Graphics/OpenGL/Textures/Textures.h
    include/Graphics/OpenGL/Textures/stb_image.h
    include/ImGui/ImGuiLayer.h
    include/ImGui/ImGuiScoped.h
    include/ImGui/ImGuiObjectState.h
    include/ImGui/TransformGizmo.h
    include/Graphics/OpenGL/Transformations/Transformations.h
    include/Math/Math.h
        include/Core/Engine.h
    include/Core/Window.h
    include/Core/File.h
    include/Core/Logger.h
    include/Input/Input.h
    include/Input/InputContext.h
        include/CallBack/CallBack.h
    include/Graphics/OpenGL/Mesh/MeshFactory.h
    include/Graphics/OpenGL/Mesh/MeshData3D.h
        include/Graphics/OpenGL/Renderer/GPURenderer.h
    include/Core/Services.h
    include/Graphics/OpenGL/Camera/Camera.h
    include/Scene/Scene.h
    include/Scene/Components.h
    include/Scene/Entity.h
    include/Core/Assert.h
    include/Core/Compat.h
    include/Core/UUID.h
    include/Core/Base.h
    include/Graphics/OpenGL/Renderer/SceneRenderer.h
)

# Include directories
target_include_directories(ZeroDayEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_compile_features(ZeroDayEngine PUBLIC cxx_std_20)

# Find vcpkg packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Link vcpkg dependencies
target_link_libraries(ZeroDayEngine PUBLIC
    glad_local
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    imgui::imgui
    assimp::assimp
)

# Configure engine config files
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
set(TEXTURES_DIR "${ASSETS_DIR}textures/")
set(MODEL_DIR "${ASSETS_DIR}models/")
set(SHADERS_CONFIG_PATH "${SHADERS_DIR}config/shaders.json")
set(MATERIAL_JSON_PATH "${ASSETS_DIR}json/")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/Core/Config.h
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Core/EngineConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/Core/EngineConfig.h
)

# Compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ZeroDayEngine PRIVATE -Wall -Wextra -Wpedantic -O2)
elseif(MSVC)
    target_compile_options(ZeroDayEngine PRIVATE /W4 /O2)
endif()

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")