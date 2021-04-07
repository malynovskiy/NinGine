cmake_minimum_required(VERSION 3.15)
project(Ningine)

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
		Headers
		Math/Headers
	PRIVATE
		Source
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	PRIVATE
		glfw
		glad::glad
		glm::glm
		OpenCL::OpenCL
)

target_sources(${PROJECT_NAME}
	PUBLIC
		Headers/NingineInit.hpp
		Headers/OpenGLContextWrapper.hpp
		Headers/OpenCLContextWrapper.hpp
		Headers/GraphicsContextBridge.hpp
		Headers/Utility.hpp
		Headers/Material.hpp
		Headers/Light.hpp
		Headers/Sphere.hpp
		Headers/Scene.hpp
		Math/Headers/Vector.hpp
		
	PRIVATE
		Source/Ningine.cpp
		Source/OpenGLContextWrapper.cpp
		Source/OpenCLContextWrapper.cpp
		Source/OpenCLUtility.cpp
		Source/CLWrapper.cpp
		Source/CLError.cpp
		Source/GLScreen.cpp
		Source/GLShaderProgram.cpp
		Source/Scene.cpp
		Math/Source/Vector.cpp

		nintracer.cpp)
