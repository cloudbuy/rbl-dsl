SET(    PARSER_COMPILE_FILES
            NamespaceParsers.cc
            )
 

ADD_LIBRARY(    event_model SHARED ${PARSER_COMPILE_FILES} )
