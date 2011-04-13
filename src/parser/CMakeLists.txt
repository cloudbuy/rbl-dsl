SET(    PARSER_COMPILE_FILES
            Descriptors.cc
            NamespaceParsers.cc
            ../../include/event_model/Descriptors.h
            ../../include/parser/NamespaceParsers.h
            ../../include/event_model/detail/Descriptors_inline.h
            ../../include/parser/detail/EventModelGrammar.h
            ../../include/event_model/detail/OidContainer.h)
 

ADD_LIBRARY(    parser SHARED ${PARSER_COMPILE_FILES} )
