cmake_minimum_required(VERSION 3.11)

#project name
project(olc-jam-2022)

set(BIN_NAME olc)

#run the executable
add_custom_target(
	run
	COMMAND ./${BIN_NAME}
)

add_subdirectory(lib/glad)
add_custom_target(
	lib_glad
	COMMAND cd lib/glad && make
)

find_package(OpenGL REQUIRED)

aux_source_directory(src src)
aux_source_directory(src/GL gl-func)
aux_source_directory(src/world world)
aux_source_directory(src/event event)
aux_source_directory(src/gameobject gameobject)

add_executable(
	${BIN_NAME}

	${src}
	${gl-func}
	${world}
	${event}
	${gameobject}
)

add_dependencies(${BIN_NAME} lib_glad)
target_link_libraries(${BIN_NAME} glfw3)
target_link_libraries(${BIN_NAME} SOIL2 OpenGL::GL)
target_link_libraries(${BIN_NAME} ${CMAKE_BINARY_DIR}/lib/glad/libglad.a)
