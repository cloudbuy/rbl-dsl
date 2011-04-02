ADD_EXECUTABLE( test_Grammar_ "test_Grammar.cc")
TARGET_LINK_LIBRARIES(test_Grammar_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE( test_Descriptors_ "test_Descriptors.cc")
TARGET_LINK_LIBRARIES(test_Descriptors_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE (test_OidContainer_ "test_OidContainer.cc")
TARGET_LINK_LIBRARIES(test_OidContainer_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE ( test_FileParsing_ "test_FileParsing.cc")
target_link_libraries( test_FileParsing_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_TEST(test_Grammar test_Grammar_ )
ADD_TEST(test_Descriptor test_Descriptors_)
ADD_TEST(test_OidContainer test_OidContainer_)
ADD_TEST(test_FileParsing test_FileParsing_)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_correct       DESTINATION ${TEST_DATA_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_failbrace     DESTINATION ${TEST_DATA_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/namespace_spacein_id    DESTINATION ${TEST_DATA_DIR})

