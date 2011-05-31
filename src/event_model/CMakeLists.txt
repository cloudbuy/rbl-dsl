SET(    PARSER_COMPILE_FILES
            DescriptorBuilders.cc
            NamespaceParsers.cc
            descriptor_karma_generators.cc
            )
 

ADD_LIBRARY(    event_model SHARED ${PARSER_COMPILE_FILES} )
