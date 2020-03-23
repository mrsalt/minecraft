include_directories(../../../../googletest/googletest/include)
add_executable(test
    main.cpp
    ring-iterator/tests.cpp
)
target_link_libraries(test gtest)