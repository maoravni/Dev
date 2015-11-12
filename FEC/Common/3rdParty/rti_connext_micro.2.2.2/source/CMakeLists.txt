cmake_minimum_required(VERSION 2.6)
project(rti_me C)

#
# Configure build options
#

option(BUILD_USE_GLOBAL_CONFIG "Enable each module include global_config.h" OFF)
option(BUILD_DEBUG_VERSION "Build for debug version" OFF)
option(BUILD_SHARED_LIBS "Build for shared libraries" ON)
option(BUILD_DYNAMIC_ENDPOINT_DISCOVERY "Build for dynamic endpoint discovery" ON)
option(BUILD_FOR_POSIX_TARGET "Build for Posix platform" ON)
option(BUILD_FOR_FREERTOS_TARGET "Build for FreeRTOS platform" OFF)
option(BUILD_FOR_VXWORKS_TARGET "Build for VxWorks platform" OFF)
option(BUILD_FOR_WINDOWS_TARGET "Build for Windows platform" OFF)
option(BUILD_FOR_STELLARIS_TARGET "Build for Stellaris platform" OFF)

if (BUILD_USE_GLOBAL_CONFIG)
    add_definitions(-DHAVE_GLOBAL_CONFIG)
endif ()

if (BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
else ()
    set(LIBRARY_TYPE STATIC)
endif ()

#
# Core modules
#
LIST(APPEND modules osapi/common)
#
# select ONE of the osapi platforms
#
if(BUILD_FOR_POSIX_TARGET)
	LIST(APPEND modules osapi/posix)
    add_definitions(-DOSAPI_PLATFORM=1)
endif ()
if(BUILD_FOR_FREERTOS_TARGET)
	LIST(APPEND modules osapi/freertos)
    add_definitions(-DOSAPI_PLATFORM=3)
endif ()
if(BUILD_FOR_VXWORKS_TARGET)
	LIST(APPEND modules osapi/vxworks)
    add_definitions(-DOSAPI_PLATFORM=4)
endif ()
if(BUILD_FOR_WINDOWS_TARGET)
	LIST(APPEND modules osapi/windows)
    add_definitions(-DOSAPI_PLATFORM=5)
endif ()
if(BUILD_FOR_STELLARIS_TARGET)
	LIST(APPEND modules osapi/stellaris)
    add_definitions(-DOSAPI_PLATFORM=6)
endif ()
LIST(APPEND modules osapi)
LIST(APPEND modules cdr)
LIST(APPEND modules reda)
LIST(APPEND modules rt)
LIST(APPEND modules db)
LIST(APPEND modules netio)
LIST(APPEND modules rtps)
LIST(APPEND modules dds_c)
#
# Optional components
#
LIST(APPEND modules rh_sm)
LIST(APPEND modules wh_sm)
if (BUILD_DYNAMIC_ENDPOINT_DISCOVERY)
    LIST(APPEND modules disc_dpde)
else ()
    LIST(APPEND modules disc_dpse)
endif ()

SET(RTI_ME_SOURCE)
SET(RTI_ME_HEADER)

#
# Create list of source files (both .c and .h)
# and create target library for this build.
#

include_directories(include)
# include headers and sources from top level rti_me module
include ("include/CMakeLists.txt" OPTIONAL)
LIST(APPEND RTI_ME_SOURCE ${rti_me_SOURCE} ${rti_me_HEADER})
# include headers and sources from component modules
FOREACH (m ${modules})
        include ("include/${m}/CMakeLists.txt" OPTIONAL)
        include ("srcC/${m}/CMakeLists.txt" OPTIONAL)
        LIST(APPEND RTI_ME_SOURCE ${${m}_SOURCE} ${${m}_HEADER})
ENDFOREACH()
add_library(rti_me ${LIBRARY_TYPE} ${RTI_ME_SOURCE})

#
# Configure options for target
#

add_definitions(-DBUILDABLE_SOURCE)
if (BUILD_DEBUG_VERSION)
    set_source_files_properties(COMPILE_FLAGS -g)
else () 
    add_definitions(-DNDEBUG)
endif ()

if (BUILD_FOR_WINDOWS_TARGET)
    add_definitions(-DRTI_WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_WIN32_WINNT=0x0501)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	target_link_libraries(rti_me ws2_32)
    if (BUILD_SHARED_LIBS)
        add_definitions(-DRTIME_DLL_EXPORT)
    endif()
endif()
