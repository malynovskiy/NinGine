cmake_minimum_required(VERSION 3.15)
project(NinGine)

add_executable(${PROJECT_NAME})

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	PRIVATE
		glfw
		glad::glad
		glm
)

target_sources(${PROJECT_NAME}
	PUBLIC
	PRIVATE
		src/graphics
		src/stb_image.cpp
		ningine.cpp)
