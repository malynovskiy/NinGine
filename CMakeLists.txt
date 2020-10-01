cmake_minimum_required(VERSION 3.15)
project(NinGine)

add_executable(${PROJECT_NAME})

set(resources_src_dir "${CMAKE_SOURCE_DIR}/resources")
set(resources_dst_dir "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${resources_src_dir} ${resources_dst_dir}
	DEPENDS ${resources_dst_dir}
	COMMENT "symbolic link resources folder from ${resources_src_dir} to ${resources_dst_dir}"
)
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


add_custom_target(copy-files ALL
	COMMAND ${CMAKE_COMMAND} -E resources
	${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/DESTINATIONDIRECTORY
    )

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
