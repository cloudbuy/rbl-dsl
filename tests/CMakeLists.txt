SET(PARSER_TESTS_ENABLED false CACHE BOOL "build the parser tests")

ADD_SUBDIRECTORY(test_processes test_processes)

IF(${PARSER_TESTS_ENABLED})
    ADD_SUBDIRECTORY(parser parser)
ENDIF()
