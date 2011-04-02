SET(    PARSER_COMPILE_FILES
            Descriptors.cc
            NamespaceParsers.cc 
            ../../include/parser/Descriptors.h
            ../../include/parser/NamespaceParsers.h
            ../../include/parser/detail/Descriptors_inline.h
            ../../include/parser/detail/EventModelGrammar.h
            ../../include/parser/detail/OidContainer.h)
 

ADD_LIBRARY(    parser SHARED ${PARSER_COMPILE_FILES} )
