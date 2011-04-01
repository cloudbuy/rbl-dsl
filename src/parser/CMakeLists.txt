SET(    PARSER_COMPILE_FILES
           Descriptors.cc
           NamespaceParsers.cc )

ADD_LIBRARY(    parser SHARED ${PARSER_COMPILE_FILES} )
