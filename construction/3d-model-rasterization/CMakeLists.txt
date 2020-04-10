#enable_testing()

include_directories(
    .
    generation
    geometry
    model-readers
    model-writers
    segmentation
    utils
)

add_library(rasterize-lib
    ModelBuilder.cpp
    generation/debug.cpp
    generation/ModelGenerator.cpp
    geometry/geometry.cpp
    geometry/LineSegment.cpp
    geometry/Point.cpp
    geometry/Polygon.cpp
    geometry/Size.cpp
    model-readers/ModelReader_Wavefront.cpp
    model-writers/ModelWriter_Wavefront.cpp
    segmentation/PolygonSegmentation.cpp
)

add_executable(rasterize-3d
    main.cpp
)

target_link_libraries(rasterize-3d rasterize-lib)

set_target_properties(rasterize-3d rasterize-lib
    PROPERTIES
    FOLDER "construction/3d-model-rasterization"
)

add_subdirectory(test)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)