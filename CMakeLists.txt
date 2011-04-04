PROJECT(SWAT)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_VERSION "1.46.1" CACHE STRING "The version of Boost used, this setting is essential to the functioning of the build system")

IF(MSVC)
    SET(DEV_BASE_DIR "c:\\dev_32_new" CACHE PATH "The base development directory.")
    
    SET(RCF_DIR "${DEV_BASE_DIR}\\RCF" CACHE PATH "THE RCF DIR")
    SET(BOOST_DIR "${DEV_BASE_DIR}\\Boost" Cache PATH "The Boost Base Directory.")
    SET(GTEST_DIR "${DEV_BASE_DIR}\\gtest-1.5.0" CACHE PATH "The Google Test Directory")
ENDIF()
IF(UNIX)
    IF(CMAKE_SIZEOF_VOID_P MATCHES 8)
        SET(DEV_BASE_DIR "/opt/dev_64_swat" CACHE PATH "The base development directory.")
    ELSE()
    ENDIF()
    SET(RCF_DIR "${DEV_BASE_DIR}/RCF/" CACHE PATH "THE RCF DIR")
    SET(BOOST_DIR "${DEV_BASE_DIR}/Boost" Cache PATH "The Boost Base Directory.")
    SET(GTEST_DIR "${DEV_BASE_DIR}/gtest" CACHE PATH "The Google Test Directory")
ENDIF()

SET(CMAKE_PREFIX_PATH ${GTEST_DIR};${BOOST_DIR};${CMAKE_PREFIX_PATH};${RCF_DIR};)
find_package(GTest)

# Boost can autolink on windows
IF(MSVC)
    find_library(RCF_LIB RCF PATH_SUFFIXES "\\lib\\Release")

ELSEIF(UNIX)
    find_library(RCF_LIB RCF)

    
    
    find_library(GTEST_STATIC_LIB libgtest.a) 
    if(GTEST_STATIC_LIB-NOTFOUND) 
        message(FATAL_ERROR "gtest static library not found") 
    else() 
        message("gtest static lib path ${GTEST_STATIC_LIB}") 
    endif()

    add_definitions(-DRCF_USE_BOOST_ASIO)
ENDIF()

if(RCF_LIB-NOTFOUND)
    message("RCF not found")
else()
    message("RCF found ${RCF_LIB}")
    INCLUDE_DIRECTORIES("${RCF_DIR}/include")
endif()

SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS   ON)
FIND_PACKAGE(   Boost ${BOOST_VERSION} REQUIRED
                COMPONENTS  system
                            filesystem
                            program_options)
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS}; ${PROTOBUF_INCLUDE_DIR} )
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})
LINK_DIRECTORIES(${RCF_DIR}/include})
INCLUDE_DIRECTORIES(src)
ENABLE_TESTING()

INCLUDE_DIRECTORIES("include")

SET( TEST_DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/test_data)

file(MAKE_DIRECTORY TEST_DATA_DIR)


ADD_SUBDIRECTORY(src modules)
ADD_SUBDIRECTORY(tests)
