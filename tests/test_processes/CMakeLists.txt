ADD_EXECUTABLE( test_server "test_server.cc")
TARGET_LINK_LIBRARIES( test_server server parser  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)

ADD_EXECUTABLE( test_client "test_client.cc")
TARGET_LINK_LIBRARIES( test_client event_model parser  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)

