cmake_minimum_required(VERSION 3.14)

project(XS4GCR
  VERSION 1.0
  LANGUAGES Fortran C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
    src/gammas/Kafexhiu2014.cpp
    src/gammas/Kamae2006.cpp
    src/gammas/Kelner2006.cpp
    src/gammas/AAfragSecGammas.cpp
    src/tertiary/AAfragTerProtons.cpp
    src/nuclei/AAfragSecNuclei.cpp
    src/fragmentations/Fluka4Dragon.cpp
    src/fragmentations/UsineFragmentation.cpp
    src/fragmentations/Webber1993.cpp
    src/core/common.cpp
    src/core/lis.cpp
    src/core/nucleiChart.cpp
    src/core/utilities.cpp
    src/core/xsecs.cpp
    external/LibppGam/LibppGam.c
    "${git_revision_cpp}"
)

# LibppGam ships as a .c file; compile it as C++ so its symbols match the C++ linkage
# expected by the (C++) caller in src/gammas/Kafexhiu2014.cpp.
set_source_files_properties(external/LibppGam/LibppGam.c PROPERTIES LANGUAGE CXX)

# PLOG (header-only, provided)
# cparamlib (provided)
add_subdirectory(external/cparamlib)
# nuclear codes (provided)
add_subdirectory(external/nuclear_codes)

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
file(GLOB XS4GCR_DATA "data/*")
file(COPY ${XS4GCR_DATA} DESTINATION ${PROJECT_BINARY_DIR}/data)

# library
add_library(XS4GCR_LIB ${XS4GCR_SRCS})
target_include_directories(XS4GCR_LIB
  PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    external/plog/include
    external/cparamlib/include
    external/LibppGam
)
target_compile_options(XS4GCR_LIB PRIVATE -ftree-vectorize -O3 -DDEBUG)
target_link_libraries(XS4GCR_LIB
  PUBLIC
    cparamlib
    nuclear_codes
    GSL::gsl
    GSL::gslcblas
    ${GFORTRAN_LIBRARIES}
)

# apps
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

add_executable(cramsInelastic apps/cramsInelastic.cpp)
target_link_libraries(cramsInelastic PRIVATE XS4GCR_LIB)

add_executable(cramsFragmentation apps/cramsFragmentation.cpp)
target_link_libraries(cramsFragmentation PRIVATE XS4GCR_LIB)

add_executable(hermesNeutrals apps/hermesNeutrals.cpp)
target_link_libraries(hermesNeutrals PRIVATE XS4GCR_LIB)

# add_executable(inelastic apps/inelastic.cpp)
# target_link_libraries(inelastic PRIVATE XS4GCR_LIB)


# install rules
install(TARGETS XS4GCR_LIB
  EXPORT XS4GCRTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include/XS4GCR)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION share/XS4GCR/data)

# summary
message(STATUS "CMAKE_SYSTEM .......... = ${CMAKE_SYSTEM}")
message(STATUS "BUILD_SHARED_LIBS ..... = ${BUILD_SHARED_LIBS}")
message(STATUS "CMAKE_CXX_COMPILER .... = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD .... = ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_INCLUDE_PATH .... = ${CMAKE_INCLUDE_PATH}")
message(STATUS "GSL_VERSION ........... = ${GSL_VERSION}")
message(STATUS "GSL_INCLUDE_DIRS ...... = ${GSL_INCLUDE_DIRS}")