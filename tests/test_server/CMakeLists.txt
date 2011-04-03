ADD_EXECUTABLE( test_server "test_server.cc")
TARGET_LINK_LIBRARIES( test_server server parser  ${RCF_LIB} ${Boost_LIBRARIES} -lpthread)
