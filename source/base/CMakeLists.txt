cmake_minimum_required(VERSION 2.8)

project ( base )
# ----------------------------------------------------------------

include ( ${CMAKE_CURRENT_LIST_DIR}/ballistic.config.cmake)

include_directories (
	${CMAKE_CURRENT_LIST_DIR}/include
)

file ( GLOB base_source
	${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
	${CMAKE_CURRENT_LIST_DIR}/include/*.inl
	${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
)

add_library ( base STATIC ${base_source} )
target_link_libraries ( base tinyxml2 )