include_directories(
    .
    ../../../../googletest/googletest/include
    ${CAIRO_INCLUDE_DIR}
)

add_executable(test-rasterize-lib
    main.cpp
    cairo-util.cpp
    polygon-segmentation-tests.cpp
    ring-iterator-tests.cpp
)

target_link_libraries(test-rasterize-lib gtest rasterize-lib ${CAIRO_LIB})

set_target_properties(test-rasterize-lib
    PROPERTIES
    FOLDER "test/construction/3d-model-rasterization"
)