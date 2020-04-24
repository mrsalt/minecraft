include_directories(
    .
    ../../../../googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/cairo
)

add_executable(test-rasterize-lib
    main.cpp
    polygon-expectations.h
    polygon-expectations.cpp
    polygon-math-tests.cpp
    polygon-segmentation-tests.cpp
    polygon-union-tests.cpp
    ring-iterator-tests.cpp
    static-polygons.h
    static-polygons.cpp
)

target_link_libraries(test-rasterize-lib gtest rasterize-lib cairo-util)

set_target_properties(test-rasterize-lib
    PROPERTIES
    FOLDER "test/construction/3d-model-rasterization"
)