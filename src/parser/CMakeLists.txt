SET(    PARSER_COMPILE_FILES
            DescriptorBuilders.cc
            NamespaceParsers.cc
            )
 

ADD_LIBRARY(    parser SHARED ${PARSER_COMPILE_FILES} )
