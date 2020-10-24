if(NOT DEFINED MATH_LIB_NAME)
	set(MATH_LIB_NAME Math)
endif()

add_library(${MATH_LIB_NAME})

set_target_properties(${MATH_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
	target_compile_options(${MATH_LIB_NAME} PRIVATE /W4)
else()
	target_compile_options(${MATH_LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${MATH_LIB_NAME}
	PUBLIC
		${CMAKE_SOURCE_DIR}/include/Math
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${MATH_LIB_NAME}
	PUBLIC
		cxx_std_17
)

target_sources(${MATH_LIB_NAME}
	PRIVATE
		# Headers
		${CMAKE_SOURCE_DIR}/include/Math/Vector.h
		${CMAKE_SOURCE_DIR}/include/Math/Point.h
		# Sources
		placeholder.cpp
)