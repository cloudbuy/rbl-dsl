SET(PARSER_TESTS_ENABLED false CACHE BOOL "build the parser tests")

ADD_SUBDIRECTORY(test_server test_server)

ADD_EXECUTABLE(temp_test "parser/test_RowAndTable.cc")
TARGET_LINK_LIBRARIES(temp_test parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

IF(${PARSER_TESTS_ENABLED})
    ADD_SUBDIRECTORY(parser parser)
ENDIF()
