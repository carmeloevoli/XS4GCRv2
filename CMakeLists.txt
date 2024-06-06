cmake_minimum_required(VERSION 3.5)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/14.1.0/bin/gcc-14")
set(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/14.1.0/bin/g++-14")
set(CMAKE_Fortran_COMPILER "/usr/local/Cellar/gcc/14.1.0/bin/gfortran-14")
set(CMAKE_CXX_FLAGS "-std=c++14 -ftree-vectorize -O3 -DDEBUG")
set(XS4GCR_INCLUDES)
set(XS4GCR_LIBRARIES)

set(LANGS Fortran C CXX)
project(XS4GCR LANGUAGES ${LANGS})
set(VERSION 1.0)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories(include)

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_HAS_LOCAL_CHANGES)
set(git_revision_cpp "${CMAKE_CURRENT_BINARY_DIR}/core/git_revision.cpp")
configure_file("src/core/git_revision.cpp.in" "${git_revision_cpp}" @ONLY)

set(XS4GCR_SRCS
	src/models/apDiMauro14.cpp
	src/models/apDuperray03.cpp
	src/models/apTanNg83.cpp
	src/models/gammaKafexhiu14.cpp
	src/models/gammaKelner06.cpp
	src/models/inTripathi99.cpp
	src/models/secAAfrag101.cpp
	src/models/secKamae06.cpp
	src/antiprotons/SecondaryAntiprotons.cpp
	src/antiprotons/AAfragSecAp.cpp
	src/antiprotons/DiMauro2014.cpp
	src/antiprotons/Duperray2003.cpp
	src/antiprotons/Feng2016.cpp
	src/antiprotons/TanNg1983.cpp
	src/antiprotons/Winkler2017.cpp
	src/antiprotons/Korsmeier2018.cpp
	src/inelastic/CROSEC.cpp
	src/inelastic/Glauber.cpp
	src/inelastic/Letaw1983.cpp
	src/inelastic/Tripathi1999.cpp
	src/leptons/SecondaryLeptons.cpp
	src/leptons/AAfragSecLeptons.cpp
	src/leptons/Kamae2006.cpp
	src/leptons/Orusa2022.cpp
	src/leptons/HuangPohl2007.cpp
	src/gammas/Kafexhiu2014.cpp
	src/gammas/Kamae2006.cpp
	src/gammas/Kelner2006.cpp
	src/gammas/AAfragSecGammas.cpp
	src/tertiary/AAfragTerProtons.cpp
	src/core/common.cpp
	src/core/lis.cpp
	src/core/nucleiChart.cpp
	src/core/utilities.cpp
	src/core/xsecs.cpp
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

# make apps
# add_executable(secAntiprotons apps/secAntiprotons.cpp)
# target_link_libraries (secAntiprotons XS4GCR_LIB ${XS4GCR_LIBRARIES})

# add_executable(secLeptons apps/secLeptons.cpp)
# target_link_libraries (secLeptons XS4GCR_LIB ${XS4GCR_LIBRARIES})

# add_executable(secGammas apps/secGammas.cpp)
# target_link_libraries (secGammas XS4GCR_LIB ${XS4GCR_LIBRARIES})

# add_executable(testBreak apps/testBreakGammas.cpp)
# target_link_libraries (testBreak XS4GCR_LIB ${XS4GCR_LIBRARIES})

#add_executable(qism apps/qism.cpp)
#target_link_libraries (qism XS4GCR_LIB ${XS4GCR_LIBRARIES})

# add_executable(terProtons apps/terProtons.cpp)
# target_link_libraries (terProtons XS4GCR_LIB ${XS4GCR_LIBRARIES})

add_executable(lightXsecs apps/lightXsecs.cpp)
target_link_libraries (lightXsecs XS4GCR_LIB ${XS4GCR_LIBRARIES})

#add_executable(inelastic apps/inelastic.cpp)
#target_link_libraries (inelastic XS4GCR_LIB ${XS4GCR_LIBRARIES})

# add_executable(decay examples/decay.cpp)
# target_link_libraries (decay XS4GCR_LIB ${XS4GCR_LIBRARIES})

#add_executable(hermes apps/hermesTables.cpp)
#target_link_libraries (hermes XS4GCR_LIB ${XS4GCR_LIBRARIES})

# Show summary.
message (STATUS "CMAKE_SYSTEM .......... = ${CMAKE_SYSTEM}")
message (STATUS "BUILD_SHARED_LIBS ..... = ${BUILD_SHARED_LIBS}")
message (STATUS "CMAKE_CXX_COMPILER .... = ${CMAKE_CXX_COMPILER}")
message (STATUS "CMAKE_CXX_FLAGS ....... = ${CMAKE_CXX_FLAGS}")
message (STATUS "CMAKE_INCLUDE_PATH .... = ${CMAKE_INCLUDE_PATH}")
message (STATUS "XS4GCR_INCLUDES ....... = ${XS4GCR_INCLUDES}")
message (STATUS "XS4GCR_LIBRARIES ...... = ${XS4GCR_LIBRARIES}")