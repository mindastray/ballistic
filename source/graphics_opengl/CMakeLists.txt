cmake_minimum_required(VERSION 2.8)

project ( graphics_opengl )
# ----------------------------------------------------------------

include_directories (
	${CMAKE_CURRENT_LIST_DIR}/include
)

file ( GLOB graphics_gl_source
	${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
	${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
)

add_library ( graphics_opengl STATIC ${graphics_gl_source} )

target_link_libraries ( graphics_opengl base graphics ${BALLISTIC_GL_LIBRARIES} glew )
