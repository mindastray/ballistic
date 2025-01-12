cmake_minimum_required(VERSION 2.8)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)
#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_LIST_DIR})
endif()

set(ZLIB_PC ${CMAKE_CURRENT_LIST_DIR}/zlib.pc)
configure_file( ${CMAKE_CURRENT_LIST_DIR}/zlib.pc.cmakein ${ZLIB_PC} @ONLY)
configure_file(	${CMAKE_CURRENT_LIST_DIR}/zconf.h.cmakein ${CMAKE_CURRENT_LIST_DIR}/zconf.h @ONLY)

include_directories( ${CMAKE_CURRENT_LIST_DIR} )

#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
    ${CMAKE_CURRENT_LIST_DIR}/zconf.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    ${CMAKE_CURRENT_LIST_DIR}/crc32.h
    ${CMAKE_CURRENT_LIST_DIR}/deflate.h
    ${CMAKE_CURRENT_LIST_DIR}/gzguts.h
    ${CMAKE_CURRENT_LIST_DIR}/inffast.h
    ${CMAKE_CURRENT_LIST_DIR}/inffixed.h
    ${CMAKE_CURRENT_LIST_DIR}/inflate.h
    ${CMAKE_CURRENT_LIST_DIR}/inftrees.h
    ${CMAKE_CURRENT_LIST_DIR}/trees.h
    ${CMAKE_CURRENT_LIST_DIR}/zutil.h
)
set(ZLIB_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/adler32.c
    ${CMAKE_CURRENT_LIST_DIR}/compress.c
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/deflate.c
    ${CMAKE_CURRENT_LIST_DIR}/gzclose.c
    ${CMAKE_CURRENT_LIST_DIR}/gzlib.c
    ${CMAKE_CURRENT_LIST_DIR}/gzread.c
    ${CMAKE_CURRENT_LIST_DIR}/gzwrite.c
    ${CMAKE_CURRENT_LIST_DIR}/inflate.c
    ${CMAKE_CURRENT_LIST_DIR}/infback.c
    ${CMAKE_CURRENT_LIST_DIR}/inftrees.c
    ${CMAKE_CURRENT_LIST_DIR}/inffast.c
    ${CMAKE_CURRENT_LIST_DIR}/trees.c
    ${CMAKE_CURRENT_LIST_DIR}/uncompr.c
    ${CMAKE_CURRENT_LIST_DIR}/zutil.c
)

if(NOT MINGW)
    set(ZLIB_DLL_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/win32/zlib1.rc # If present will override custom build rule below.
    )
endif()

if(CMAKE_COMPILER_IS_GNUCC)
    if(ASM686)
        set(ZLIB_ASMS ${CMAKE_CURRENT_LIST_DIR}/contrib/asm686/match.S)
    elseif (AMD64)
        set(ZLIB_ASMS ${CMAKE_CURRENT_LIST_DIR}/contrib/amd64/amd64-match.S)
    endif ()

	if(ZLIB_ASMS)
		add_definitions(-DASMV)
		set_source_files_properties(${ZLIB_ASMS} PROPERTIES LANGUAGE C COMPILE_FLAGS -DNO_UNDERLINE)
	endif()
endif()

if(MSVC)
    if(ASM686)
		ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS
			${CMAKE_CURRENT_LIST_DIR}/contrib/masmx86/inffas32.asm
			${CMAKE_CURRENT_LIST_DIR}/contrib/masmx86/match686.asm
		)
    elseif (AMD64)
		ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS
			${CMAKE_CURRENT_LIST_DIR}/contrib/masmx64/gvmat64.asm
			${CMAKE_CURRENT_LIST_DIR}/contrib/masmx64/inffasx64.asm
		)
    endif()

	if(ZLIB_ASMS)
		add_definitions(-DASMV -DASMINF)
	endif()
endif()

add_library(zlib STATIC ${ZLIB_SRCS} ${ZLIB_ASMS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})

set_property (TARGET zlib PROPERTY FOLDER "deps")