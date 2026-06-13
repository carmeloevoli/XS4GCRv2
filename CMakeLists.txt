cmake_minimum_required(VERSION 3.14)

project(XS4GCR
  VERSION 1.0
  LANGUAGES Fortran C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(XS4GCR_BUILD_APPS "Build XS4GCR command-line applications" ON)
option(XS4GCR_INSTALL_DATA "Install XS4GCR data tables" ON)
option(XS4GCR_ENABLE_WARNINGS "Enable a broad set of compiler warnings" ON)
option(XS4GCR_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

function(xs4gcr_enable_target_warnings target)
  if(NOT XS4GCR_ENABLE_WARNINGS)
    return()
  endif()

  if(MSVC)
    target_compile_options("${target}" PRIVATE
      "$<$<COMPILE_LANGUAGE:C>:/W4>"
      "$<$<COMPILE_LANGUAGE:CXX>:/W4>"
    )
  else()
    target_compile_options("${target}" PRIVATE
      "$<$<COMPILE_LANGUAGE:C>:-Wall;-Wpedantic>"
      "$<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wpedantic>"
      "$<$<COMPILE_LANGUAGE:Fortran>:-Wall;-Wpedantic>"
    )
  endif()

  if(XS4GCR_WARNINGS_AS_ERRORS)
    if(MSVC)
      target_compile_options("${target}" PRIVATE /WX)
    else()
      target_compile_options("${target}" PRIVATE -Werror)
    endif()
  endif()
endfunction()

if(NOT MSVC)
  check_cxx_compiler_flag("-ftree-vectorize" XS4GCR_HAS_FTREE_VECTORIZE)
endif()

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_HAS_LOCAL_CHANGES)
set(git_revision_cpp "${CMAKE_CURRENT_BINARY_DIR}/core/git_revision.cpp")
configure_file("src/core/git_revision.cpp.in" "${git_revision_cpp}" @ONLY)

set(XS4GCR_SRCS
    src/shared/secAAfrag202.cpp
    src/shared/secKamae06.cpp
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
    src/neutrals/Kafexhiu2014.cpp
    src/neutrals/Kamae2006.cpp
    src/neutrals/Kelner2006.cpp
    src/neutrals/AAfragSecNeutrals.cpp
    src/nuclei/AAfragSecNuclei.cpp
    src/fragmentations/Evoli2026.cpp
    src/core/fitting_functions.cpp
    src/core/ghost_tree.cpp
    src/fragmentations/Fluka4Dragon.cpp
    src/fragmentations/UsineFragmentationTables.cpp
    src/fragmentations/Webber1993.cpp
    src/core/common.cpp
    src/core/lis.cpp
    src/core/utilities.cpp
    src/core/xsecs.cpp
    external/LibppGam/LibppGam.c
    "${git_revision_cpp}"
)

# LibppGam ships as a .c file; compile it as C++ so its symbols match the C++ linkage
# expected by the (C++) caller in src/neutrals/Kafexhiu2014.cpp.
set_source_files_properties(external/LibppGam/LibppGam.c PROPERTIES LANGUAGE CXX)

# PLOG (header-only, provided)
# cparamlib (provided)
add_subdirectory(external/cparamlib)
# nuclear codes (provided)
add_subdirectory(external/nuclear_codes)

target_include_directories(cparamlib
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/cparamlib/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

xs4gcr_enable_target_warnings(cparamlib)
xs4gcr_enable_target_warnings(nuclear_codes)

# gfortran runtime
set(GFORTRAN_LIBRARIES "")
if(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
    execute_process(
        COMMAND ${CMAKE_Fortran_COMPILER} -print-file-name=libgfortran${CMAKE_SHARED_LIBRARY_SUFFIX}
        OUTPUT_VARIABLE GFORTRAN_RUNTIME
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(EXISTS "${GFORTRAN_RUNTIME}")
        list(APPEND GFORTRAN_LIBRARIES "${GFORTRAN_RUNTIME}")
    else()
        list(APPEND GFORTRAN_LIBRARIES gfortran)
    endif()
endif()

# GSL (external)
find_package(GSL REQUIRED)

# create output dir in build tree
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# copy data files to build directory
file(COPY "${PROJECT_SOURCE_DIR}/data"
     DESTINATION "${PROJECT_BINARY_DIR}"
     PATTERN ".DS_Store" EXCLUDE)

# library
add_library(XS4GCR_LIB ${XS4GCR_SRCS})
add_library(XS4GCR::XS4GCR ALIAS XS4GCR_LIB)
set_target_properties(XS4GCR_LIB
  PROPERTIES
    OUTPUT_NAME XS4GCR
    EXPORT_NAME XS4GCR
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_compile_features(XS4GCR_LIB PUBLIC cxx_std_14)
target_include_directories(XS4GCR_LIB
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${PROJECT_SOURCE_DIR}/external/LibppGam
)
target_include_directories(XS4GCR_LIB SYSTEM
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/plog/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(XS4GCR_LIB PRIVATE DEBUG)
if(XS4GCR_HAS_FTREE_VECTORIZE)
  target_compile_options(XS4GCR_LIB PRIVATE $<$<CONFIG:Release>:-ftree-vectorize>)
endif()
xs4gcr_enable_target_warnings(XS4GCR_LIB)
target_link_libraries(XS4GCR_LIB
  PUBLIC
    GSL::gsl
    GSL::gslcblas
  PRIVATE
    cparamlib
    nuclear_codes
    ${GFORTRAN_LIBRARIES}
)

# apps
set(XS4GCR_APPS
    cramsInelastic
    cramsFragmentation
    frag_table
    hermesNeutrals
)

if(XS4GCR_BUILD_APPS)
  foreach(app IN LISTS XS4GCR_APPS)
    add_executable("${app}" "apps/${app}.cpp")
    target_link_libraries("${app}" PRIVATE XS4GCR_LIB)
    xs4gcr_enable_target_warnings("${app}")
  endforeach()
endif()

# add_executable(secAntiprotons apps/secAntiprotons.cpp)
# target_link_libraries(secAntiprotons PRIVATE XS4GCR_LIB)

# add_executable(secLeptons apps/secLeptons.cpp)
# target_link_libraries(secLeptons PRIVATE XS4GCR_LIB)

# add_executable(secGammas apps/secGammas.cpp)
# target_link_libraries(secGammas PRIVATE XS4GCR_LIB)

# add_executable(testBreak apps/testBreakGammas.cpp)
# target_link_libraries(testBreak PRIVATE XS4GCR_LIB)

# add_executable(qism apps/qism.cpp)
# target_link_libraries(qism PRIVATE XS4GCR_LIB)

# add_executable(terProtons apps/terProtons.cpp)
# target_link_libraries(terProtons PRIVATE XS4GCR_LIB)

# add_executable(lightXsecs apps/lightXsecs.cpp)
# target_link_libraries(lightXsecs PRIVATE XS4GCR_LIB)

# add_executable(inelastic apps/inelastic.cpp)
# target_link_libraries(inelastic PRIVATE XS4GCR_LIB)


# install rules
set(XS4GCR_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/XS4GCR"
    CACHE STRING "CMake package config install directory")

install(TARGETS XS4GCR_LIB cparamlib nuclear_codes
  EXPORT XS4GCRTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

if(XS4GCR_BUILD_APPS)
  install(TARGETS ${XS4GCR_APPS}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  )
endif()

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/external/plog/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/external/cparamlib/include/cparamlib.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

if(XS4GCR_INSTALL_DATA)
  install(DIRECTORY "${PROJECT_SOURCE_DIR}/data/"
          DESTINATION "${CMAKE_INSTALL_DATADIR}/XS4GCR/data"
          PATTERN ".DS_Store" EXCLUDE)
endif()

install(EXPORT XS4GCRTargets
  NAMESPACE XS4GCR::
  DESTINATION "${XS4GCR_INSTALL_CMAKEDIR}"
)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/XS4GCRConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/XS4GCRConfig.cmake"
  INSTALL_DESTINATION "${XS4GCR_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/XS4GCRConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${PROJECT_BINARY_DIR}/XS4GCRConfig.cmake"
  "${PROJECT_BINARY_DIR}/XS4GCRConfigVersion.cmake"
  DESTINATION "${XS4GCR_INSTALL_CMAKEDIR}"
)

# summary
message(STATUS "CMAKE_SYSTEM .......... = ${CMAKE_SYSTEM}")
message(STATUS "BUILD_SHARED_LIBS ..... = ${BUILD_SHARED_LIBS}")
message(STATUS "CMAKE_BUILD_TYPE ...... = ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER .... = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD .... = ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_INCLUDE_PATH .... = ${CMAKE_INCLUDE_PATH}")
message(STATUS "GSL_VERSION ........... = ${GSL_VERSION}")
message(STATUS "GSL_INCLUDE_DIRS ...... = ${GSL_INCLUDE_DIRS}")
message(STATUS "XS4GCR_BUILD_APPS ..... = ${XS4GCR_BUILD_APPS}")
message(STATUS "XS4GCR_INSTALL_DATA ... = ${XS4GCR_INSTALL_DATA}")
message(STATUS "XS4GCR_ENABLE_WARNINGS  = ${XS4GCR_ENABLE_WARNINGS}")
