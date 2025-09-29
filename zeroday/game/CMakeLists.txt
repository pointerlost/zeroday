add_executable (ZeroDayGame
	src/Core/Game.cpp
	src/Core/main.cpp
	src/objects/GameObject.cpp
)

# Link engine
target_link_libraries(ZeroDayGame PRIVATE ZeroDayEngine)

target_compile_features(ZeroDayGame PUBLIC cxx_std_20)

target_include_directories(ZeroDayGame
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)