PROJECT(SWAT)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_VERSION "1.46.1" CACHE STRING "The version of Boost used, this setting is essential to the functioning of the build system")

SET(SWAT_BASE_NAMESPACE_BEGIN "namespace ukplc  { namespace swat {" CACHE STRING "THE BASE NAMESPACE USED FOR THE SWAT LIBARY")
SET(SWAT_BASE_NAMESPACE_END "} }" CACHE STRING "THE AMOUNT OF TERMINAL BRACES TO TERMINATE THE BASE NAMESPACE DEFINITION")

IF(MSVC)
ENDIF()
IF(UNIX)
    IF(CMAKE_SIZEOF_VOID_P MATCHES 8)
        SET(DEV_BASE_DIR "/opt/dev_64_swat" CACHE PATH "The base development directory.")
    ELSE()
    ENDIF()
    SET(RCF_DIR "${DEV_BASE_DIR}/RCF/" CACHE PATH "THE RCF DIR")
    SET(BOOST_DIR "${DEV_BASE_DIR}/Boost" Cache PATH "The Boost Base Directory.")
    SET(GTEST_DIR "${DEV_BASE_DIR}/gtest" CACHE PATH "The Google Test Directory")
    SET(PROTOBUF_DIR "${DEV_BASE_DIR}/Protobuf" CACHE PATH "The Protobuf Dir")
ENDIF()

SET(CMAKE_PREFIX_PATH ${GTEST_DIR};${BOOST_DIR};${CMAKE_PREFIX_PATH};${PROTOBUF_DIR};${RCF_DIR};)
find_package(GTest)
find_package(Protobuf REQUIRED)
    
find_library(RCF_LIB RCF-1.3)
if(RCF_LIB-NOTFOUND)
    message("RCF not found")
else()
    message("RCF found ${RCF_LIB}")
    INCLUDE_DIRECTORIES("${RCF_DIR}/include")
endif()

EXECUTE_PROCESS(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --version OUTPUT_VARIABLE PBUF_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message("pbuf version: " ${PBUF_VERSION})

# Boost can autolink on windows
IF(MSVC)
ELSEIF(UNIX)
    SET(Boost_USE_MULTITHREADED ON)
    SET(Boost_USE_STATIC_LIBS   ON)
    FIND_PACKAGE(   Boost ${BOOST_VERSION} REQUIRED
        COMPONENTS  system
                    filesystem
                    program_options)
    LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
    
    find_library(GTEST_STATIC_LIB libgtest.a) 
    if(GTEST_STATIC_LIB-NOTFOUND) 
        message(FATAL_ERROR "gtest static library not found") 
    else() 
        message("gtest static lib path ${GTEST_STATIC_LIB}") 
    endif()

    FIND_LIBRARY(PROTOBUF_LITE_LIBRARY libprotobuf-lite.a)
    IF(PROTOBUF_LITE_LIBRARY-NOTFOUND)
        MESSAGE(FATAL_ERROR "protobuf lite static library not found")
    ELSE()
        MESSAGE("protobuf-lite path ${PROTOBUF_LITE_LIBRARY}")
    ENDIF()
    add_definitions(-DRCF_USE_BOOST_ASIO)
ENDIF()

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS}; ${PROTOBUF_INCLUDE_DIR} )
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})
LINK_DIRECTORIES(${RCF_DIR}/include})
INCLUDE_DIRECTORIES(src)
ENABLE_TESTING()

INCLUDE_DIRECTORIES("include")

SET( TEST_DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/test_data)

file(MAKE_DIRECTORY TEST_DATA_DIR)


ADD_SUBDIRECTORY(src modules)
ADD_SUBDIRECTORY(ll_tests ll_tests)
ADD_SUBDIRECTORY(tests)
