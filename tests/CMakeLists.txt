SET(EVENT_MODEL_TESTS_ENABLED false CACHE BOOL "build the parser tests")
SET(EVENT_PROCESSING_TESTS_EVABLED false CACHE BOOL "build the processing tests")

#ADD_SUBDIRECTORY(test_processes test_processes)

IF(${EVENT_MODEL_TESTS_ENABLED})
    ADD_SUBDIRECTORY(event_model event_model)
ENDIF()

IF(${EVENT_PROCESSING_TESTS_ENABLED})
  ADD_SUBDIRECTORY(event_processing event_processing)
ENDIF()

