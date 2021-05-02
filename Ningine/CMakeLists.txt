cmake_minimum_required(VERSION 3.16)
project(Ningine CXX)

add_executable(${PROJECT_NAME})

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else ()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

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
		Headers/Ningine.hpp
		Headers/GLContextWrapper.hpp
		Headers/GLScreen.hpp
		Headers/GLShaderProgram.hpp
		Headers/CLContextWrapper.hpp
		Headers/CLUtility.hpp
		Headers/CLError.hpp
		Headers/GraphicsContextBridge.hpp
		Headers/Utility.hpp
		Headers/Material.hpp
		Headers/Light.hpp
		Headers/Sphere.hpp
		Headers/Scene.hpp
		Math/Headers/Vector.hpp
		Math/Headers/Matrix.hpp
		
	PRIVATE
		Source/Ningine.cpp
		Source/GLContextWrapper.cpp
		Source/GLScreen.cpp
		Source/GLShaderProgram.cpp
		Source/CLContextWrapper.cpp
		Source/CLUtility.cpp
		Source/CLWrapper.cpp
		Source/CLError.cpp
		Source/Scene.cpp
		Math/Source/Vector.cpp
		Math/Source/Matrix.cpp

		nintracer.cpp)
