cmake_minimum_required(VERSION 3.0)

set(LANGS Fortran C CXX)
project(XS4GCR LANGUAGES ${LANGS})

set(CMAKE_VERBOSE_MAKEFILE off )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-10")
set(CMAKE_CXX_FLAGS "-std=c++14 -ftree-vectorize -O3 -DDEBUG")
set(XS4GCR_INCLUDES)
set(XS4GCR_LIBRARIES)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories(include)

file(STRINGS ${CMAKE_SOURCE_DIR}/VERSION VERSION)

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_HAS_LOCAL_CHANGES)
set(git_revision_cpp "${CMAKE_CURRENT_BINARY_DIR}/git_revision.cpp")
configure_file("src/git_revision.cpp.in" "${git_revision_cpp}" @ONLY)

set(XS4GCR_SRCS
	src/models/apDiMauro14.cpp
	src/models/apDuperray03.cpp
	src/models/apTanNg83.cpp
	src/models/inTripathi99.cpp
	src/antiprotons/SecondaryAntiprotons.cpp
	src/antiprotons/AAfrag.cpp
	src/antiprotons/DiMauro2014.cpp
	src/antiprotons/Duperray2003.cpp
	src/antiprotons/Feng2016.cpp
	src/antiprotons/TanNg1983.cpp
	src/antiprotons/Winkler2017.cpp
	src/inelastic/CROSEC.cpp
	src/inelastic/Letaw1983.cpp
	src/inelastic/Tripathi1999.cpp
	src/leptons/SecondaryLeptons.cpp
	src/leptons/Kamae2006.cpp
	src/leptons/HuangPohl2007.cpp
	src/common.cpp
	src/nucleiChart.cpp
	src/utilities.cpp
	src/XS4GCR.cpp
    "${git_revision_cpp}"
   )

# PLOG (provided - header only)
include_directories(external/plog/include)

# cparamlib (provided)
add_subdirectory(external/cparamlib)
list(APPEND XS4GCR_LIBRARIES cparamlib)
list(APPEND XS4GCR_INCLUDES external/cparamlib/include)

# nuclear codes (provided)
add_subdirectory(external/nuclear_codes)
list(APPEND XS4GCR_LIBRARIES nuclear_codes)

# GSL (external)
find_package(GSL)
if(GSL_FOUND)
	set(HAVE_LIBGSL 1)
	list(APPEND XS4GCR_INCLUDES ${GSL_INCLUDE_DIR})
	list(APPEND XS4GCR_LIBRARIES ${GSL_LIBRARIES})
else(GSL_FOUND)
	message(FATAL_ERROR "Require GSL. Set GSL_HOME")
endif(GSL_FOUND)

# create the output dir
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# copy data files in build
file(GLOB XS4GCR_DATA "data/*")
file(COPY ${XS4GCR_DATA} DESTINATION ${PROJECT_BINARY_DIR}/data)

# make library
include_directories(include ${XS4GCR_INCLUDES})
add_library(XS4GCR_LIB ${XS4GCR_SRCS} ${XS4GCR_INCLUDES} )

# make executable
add_executable(sandbox src/sandbox.cpp)
target_link_libraries (sandbox XS4GCR_LIB ${XS4GCR_LIBRARIES})

# make examples
add_executable(antiprotons examples/antiprotons.cpp)
target_link_libraries (antiprotons XS4GCR_LIB ${XS4GCR_LIBRARIES})

add_executable(leptons examples/leptons.cpp)
target_link_libraries (leptons XS4GCR_LIB ${XS4GCR_LIBRARIES})

add_executable(inelastic examples/inelastic.cpp)
target_link_libraries (inelastic XS4GCR_LIB ${XS4GCR_LIBRARIES})

add_executable(decay examples/decay.cpp)
target_link_libraries (decay XS4GCR_LIB ${XS4GCR_LIBRARIES})

# Show summary.
message (STATUS "CMAKE_SYSTEM .......... = ${CMAKE_SYSTEM}")
message (STATUS "BUILD_SHARED_LIBS ..... = ${BUILD_SHARED_LIBS}")
message (STATUS "CMAKE_CXX_COMPILER .... = ${CMAKE_CXX_COMPILER}")
message (STATUS "CMAKE_CXX_FLAGS ....... = ${CMAKE_CXX_FLAGS}")
message (STATUS "CMAKE_INCLUDE_PATH .... = ${CMAKE_INCLUDE_PATH}")
message (STATUS "XS4GCR_INCLUDES ....... = ${XS4GCR_INCLUDES}")
message (STATUS "XS4GCR_LIBRARIES ...... = ${XS4GCR_LIBRARIES}")