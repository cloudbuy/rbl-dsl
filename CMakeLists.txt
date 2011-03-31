SET(    PARSER_COMPILE_FILES
            src/parser/Descriptors.cc
            src/parser/NamespaceParsers.cc )

SET( SERVER_COMPILE_FILES
        src/server/RelayToServer.cc)

#SET( SERVER_COMPILE_FILES
 #   src/server/main.cc)

INCLUDE_DIRECTORIES("include")

ADD_LIBRARY(    parser STATIC ${PARSER_COMPILE_FILES} )
ADD_LIBRARY(    server STATIC ${SERVER_COMPILE_FILES})

ADD_EXECUTABLE( test_server "src/server/test_server.cc")
TARGET_LINK_LIBRARIES( test_server server parser  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)



ADD_EXECUTABLE( test_Grammar_ "ll_tests/parser/test_Grammar.cc")
TARGET_LINK_LIBRARIES(test_Grammar_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE( test_Descriptors_ "ll_tests/parser/test_Descriptors.cc")
TARGET_LINK_LIBRARIES(test_Descriptors_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE (test_OidContainer_ "ll_tests/parser/test_OidContainer.cc")
TARGET_LINK_LIBRARIES(test_OidContainer_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE ( test_FileParsing_ "ll_tests/parser/test_FileParsing.cc")
TARGET_LINK_LIBRARIES(test_FileParsing_ parser ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

ADD_TEST(test_Grammar test_Grammar_)
ADD_TEST(test_Descriptor test_Descriptors_)
ADD_TEST(test_OidContainer test_OidContainer_)
ADD_TEST(test_FileParsing test_FileParsing_ )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ll_tests/parser/test_data/namespace_correct  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ll_tests/parser/test_data/namespace_failbrace  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ll_tests/parser/test_data/namespace_spacein_id  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_data)
