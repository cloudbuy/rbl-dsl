ADD_EXECUTABLE( test_server "test_server.cc")
TARGET_LINK_LIBRARIES( test_server server event_model  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE( test_client "test_client.cc")
TARGET_LINK_LIBRARIES( test_client event_model event_model  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)

