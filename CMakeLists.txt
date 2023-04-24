cmake_minimum_required(VERSION 3.11)

include_directories(glad/include)

link_directories(${CMAKE_BINARY_DIR})
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_FLAGS "-O2 -static-libgcc -static-libstdc++ -mwindows -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")	
else()
	set(CMAKE_CXX_FLAGS "-O2 -static-libgcc -static-libstdc++")
endif()

#project name
project(olc-jam-2022)

set(BIN_NAME turbulence_ahead)

#run the executable
add_custom_target(
	run
	COMMAND ./${BIN_NAME}
)

find_package(OpenGL REQUIRED)

aux_source_directory(src src)
aux_source_directory(src/GL gl-func)
aux_source_directory(src/world world)
aux_source_directory(src/event event)
aux_source_directory(src/gameobject gameobject)
aux_source_directory(src/Audio audio)

add_executable(
	${BIN_NAME}

	${src}
	${gl-func}
	${world}
	${event}
	${gameobject}
	${audio}
	glad/src/glad.c
)

target_link_libraries(${BIN_NAME} glfw3)
target_link_libraries(${BIN_NAME} SOIL2 OpenGL::GL)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(${BIN_NAME} OpenAL32 sndfile)
else()
	target_link_libraries(${BIN_NAME} openal sndfile)
endif()
