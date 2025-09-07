set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable (game
	src/core/Game.cpp
	src/core/main.cpp
	src/objects/GameObject.cpp
)

# Link engine
target_link_libraries(game PRIVATE engine)

target_include_directories(game
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)