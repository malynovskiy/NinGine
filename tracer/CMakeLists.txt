cmake_minimum_required(VERSION 3.15)
project(NinTracer)

add_executable(${PROJECT_NAME})

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenCL REQUIRED)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
	PRIVATE
		source
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	PRIVATE
		glfw
		glad::glad
		glm
		OpenCL::OpenCL
)

target_sources(${PROJECT_NAME}
	PUBLIC
	PRIVATE
		include/Vector.hpp
		include/Material.hpp
		include/Light.hpp
		include/Sphere.hpp

		source/CLError.cpp
		source/CLWrapper.cpp
		source/GLScreen.cpp
		source/GLShaderProgram.cpp
		source/Ningine.cpp
		source/OpenCLUtility.cpp

		nintracer.cpp)
