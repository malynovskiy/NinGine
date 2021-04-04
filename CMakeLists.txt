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
		include
	PRIVATE
		source
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
		include/NingineInit.hpp
		include/OpenGLContextWrapper.hpp
		include/OpenCLContextWrapper.hpp
		include/GraphicsContextBridge.hpp
		include/Vector.hpp
		include/Utility.hpp
		include/Material.hpp
		include/Light.hpp
		include/Sphere.hpp
		include/Scene.hpp
		
	PRIVATE
		source/Ningine.cpp
		source/OpenGLContextWrapper.cpp
		source/OpenCLContextWrapper.cpp
		source/OpenCLUtility.cpp
		source/CLWrapper.cpp
		source/CLError.cpp
		source/GLScreen.cpp
		source/GLShaderProgram.cpp
		source/Vector.cpp
		source/Scene.cpp

		nintracer.cpp)
