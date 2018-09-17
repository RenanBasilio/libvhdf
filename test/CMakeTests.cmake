set(TEST_EXE tests)
add_executable(${TEST_EXE} test/main.cpp)
target_link_libraries(${TEST_EXE} ${LIB_NAME})
install(TARGETS ${TEST_EXE} DESTINATION bin)