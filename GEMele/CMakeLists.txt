cmake_minimum_required(VERSION 2.1 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "-std=C++11 ${CMAKE_CXX_FLAGS}")

project(discharge)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
include_directories(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

# GEMevent
ROOT_GENERATE_DICTIONARY(G__GEMEvent GEMEvent.hpp LINKDEF LinkDef.h)
add_library(GEMEvent SHARED GEMEvent.hpp G__GEMEvent)
target_link_libraries(GEMEvent ${ROOT_LIBRARIES})

#GEM
ROOT_GENERATE_DICTIONARY(G__GEM GEM.h LINKDEF GEMLinkDef.h)
add_library(GEM SHARED src/GEM.cxx G__GEM.cxx)
target_link_libraries(GEM ${ROOT_LIBRARIES})

#Hit
ROOT_GENERATE_DICTIONARY(G__HitEvent HitEvent.hpp LINKDEF HitLinkDef.h)
add_library(HitEvent SHARED HitEvent.hpp G__HitEvent)
target_link_libraries(HitEvent ${ROOT_LIBRARIES})

add_executable(GEMdiffusion GEMdiffusion.C include/GEM.h src/GEM.cxx)
target_link_libraries(GEMdiffusion GEMEvent)
target_link_libraries(GEMdiffusion HitEvent)

add_executable(qCrit Q_crit.C)
target_link_libraries(qCrit HitEvent)

add_executable(qCrit_bragg Q_crit_bragg.C)
target_link_libraries(qCrit_bragg HitEvent)

add_executable(dEdx src/dEdx.cxx)
target_link_libraries(dEdx GEMEvent)
target_link_libraries(dEdx HitEvent)
target_link_libraries(dEdx GEM)

