ADD_EXECUTABLE ( test_RelayNamespaceSerialization_ "test_RelayNamespaceSerialization.cc")
target_link_libraries( test_RelayNamespaceSerialization_  ${RCF_LIB} ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread)
