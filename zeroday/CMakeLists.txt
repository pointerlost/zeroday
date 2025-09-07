cmake_minimum_required(VERSION 3.21)
project(throwEngine)

# Optional: Clone vcpkg only if it's not already cloned
set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/vcpkg")

if(NOT EXISTS "${VCPKG_DIR}/.git")
    message(STATUS "Cloning vcpkg...")
    execute_process(COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_DIR})
endif()

# Optional: bootstrap if not built yet
if(NOT EXISTS "${VCPKG_DIR}/vcpkg")
    if(WIN32)
        execute_process(COMMAND powershell -Command "${VCPKG_DIR}/bootstrap-vcpkg.bat")
    else()
        execute_process(COMMAND ${VCPKG_DIR}/bootstrap-vcpkg.sh)
    endif()
endif()

# Add subdirectories
add_subdirectory(engine)
add_subdirectory(game)
