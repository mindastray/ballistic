cmake_minimum_required(VERSION 2.8)

project ( array_var )


set ( proj_dir ${CMAKE_CURRENT_LIST_DIR} )



include_directories (
	${proj_dir}/include
)

file ( GLOB source
	${proj_dir}/*.cpp
	${proj_dir}/*.h
	${proj_dir}/*.hpp
)

add_executable ( array_var  ${source} )

# enable c++11
if (NOT ${CMAKE_CXX_COMPILER_ID} EQUAL "MSVC")
	if (CMAKE_GENERATOR STREQUAL Xcode)
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	else ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -g -Wall")
	endif ()
endif ()

project ( array_var )
