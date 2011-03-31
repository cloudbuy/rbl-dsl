SET(    PARSER_COMPILE_FILES
            src/parser/Descriptors.cc
            src/parser/NamespaceParsers.cc )

INCLUDE_DIRECTORIES("include")

ADD_LIBRARY(    parser STATIC ${PARSER_COMPILE_FILES} )

ADD_EXECUTABLE( test_Parser_ "ll_tests/parser/test_Parser.cc")
TARGET_LINK_LIBRARIES(test_Parser_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE( test_Descriptors_ "ll_tests/parser/test_Descriptors.cc")
TARGET_LINK_LIBRARIES(test_Descriptors_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE (test_OidContainer_ "ll_tests/parser/test_OidContainer.cc")
TARGET_LINK_LIBRARIES(test_OidContainer_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_TEST(test_Parser test_Parser_)
ADD_TEST(test_Descriptor test_Descriptors_)
ADD_TEST(test_OidContainer test_OidContainer_)
