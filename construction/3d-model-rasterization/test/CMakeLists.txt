include_directories(
    .
    ../../../../googletest/googletest/include
    ${CAIRO_INCLUDE_DIR}
)

add_executable(test
    main.cpp
    cairo-util.cpp
    polygon-segmentation/tests.cpp
    ring-iterator/tests.cpp
)

target_link_libraries(test gtest rasterize-lib ${CAIRO_LIB})