SET (PARSER_TEST_FILES
    test_OidContainer.cc
    test_Grammar.cc
    test_FileParsing.cc
    test_Descriptors.cc
    test_Generator.cc
    em_tests.cc
)
     

ADD_EXECUTABLE( em_tests_ ${PARSER_TEST_FILES})
TARGET_LINK_LIBRARIES(em_tests_ event_model  ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)


ADD_TEST(em_tests em_tests_)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_correct       DESTINATION "${TEST_DATA_DIR}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_failbrace     DESTINATION "${TEST_DATA_DIR}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_spacein_id    DESTINATION "${TEST_DATA_DIR}")

