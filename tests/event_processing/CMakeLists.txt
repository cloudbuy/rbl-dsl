SET (EVENT_PROCESSING_TEST_FILES
  test_EventGenSite.cc
  ep_tests.cc
)

ADD_EXECUTABLE( ep_tests_ ${EVENT_PROCESSING_TEST_FILES})
TARGET_LINK_LIBRARIES(ep_tests_  event_model event_processing ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)

