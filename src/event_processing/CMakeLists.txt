SET(    EVENT_PROCESSING_COMPILE_FILES
            event_string_parser.cc
            event_string_generator.cc
            EventSet.cc)

ADD_LIBRARY( event_processing SHARED ${EVENT_PROCESSING_COMPILE_FILES})
