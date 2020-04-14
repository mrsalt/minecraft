include_directories(
    .
    ../construction/3d-model-rasterization/geometry
    ../construction/3d-model-rasterization
    ${CAIRO_INCLUDE_DIR}
)

set (SRCS
    cairo-util.h
    cairo-util.cpp
)

add_library(cairo-util ${SRCS})

target_link_libraries(cairo-util ${CAIRO_LIB})

set_target_properties(cairo-util
    PROPERTIES
    FOLDER "cairo"
)
