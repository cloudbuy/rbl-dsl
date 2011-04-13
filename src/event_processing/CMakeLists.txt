SET(    EVENT_PROCESSING_COMPILE_FILES
            event_string_processing.cc
            EventSet.cc)

ADD_LIBRARY( event_processing SHARED ${EVENT_PROCESSING_COMPILE_FILES})
