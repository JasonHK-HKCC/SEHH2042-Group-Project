add_executable(JetAssign)

target_sources(JetAssign PRIVATE JetAssign.cpp)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(JetAssign PUBLIC _DEBUG)
endif()
