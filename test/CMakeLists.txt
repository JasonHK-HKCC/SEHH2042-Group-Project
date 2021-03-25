# include(GoogleTest)

add_executable(JetAssign-Test)

target_sources(JetAssign-Test PRIVATE main.cpp JetAssign_UnitTest.cpp)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(JetAssign-Test PRIVATE _TEST)
endif()

target_include_directories(JetAssign-Test PUBLIC ../extern/Catch2 ../src)

# target_link_libraries(JetAssign-Test PUBLIC gtest)

# gtest_add_tests(
#     TARGET      JetAssign-Test
#     TEST_SUFFIX .noArgs
#     TEST_LIST   noArgsTests)

# set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
