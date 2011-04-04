ADD_EXECUTABLE( parser_tests_ "parser_tests.cc")
TARGET_LINK_LIBRARIES(parser_tests_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_TEST(parser_tests parser_tests_)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_correct       DESTINATION "${TEST_DATA_DIR}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_failbrace     DESTINATION "${TEST_DATA_DIR}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_spacein_id    DESTINATION "${TEST_DATA_DIR}")

