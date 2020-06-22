#enable_testing()

include_directories(
    .
    generation
    geometry
    model-readers
    model-writers
    segmentation
    utils
    ${CMAKE_SOURCE_DIR}/cairo
)

set (SRCS
    CubeModel.h
    CubeModel.cpp
    ModelBuilder.h
    ModelBuilder.cpp
    generation/debug.h
    generation/debug.cpp
    generation/ModelGenerator.h
    generation/ModelGenerator.cpp
    geometry/geometry.h
    geometry/geometry.cpp
    geometry/LineSegment.h
    geometry/LineSegment.cpp
    geometry/Point.h
    geometry/Point.cpp
    geometry/Polygon.h
    geometry/Polygon.cpp
    geometry/Rectangle.h
    geometry/Size.h
    geometry/Size.cpp
    geometry/Surface.h
    model-readers/ModelReader_Wavefront.cpp
    model-writers/ModelWriter_Wavefront.cpp
    segmentation/PolygonMath.h
    segmentation/PolygonMath.cpp
    segmentation/PolygonSegmentation.h
    segmentation/PolygonSegmentation.cpp
    segmentation/PolygonUnion.h
    segmentation/PolygonUnion.cpp
)

if (MSVC)
    list(APPEND SRCS
        debugger-visualizer.natvis
    )
endif ()

add_library(rasterize-lib ${SRCS})

add_executable(rasterize-3d
    main.cpp
)

target_link_libraries(rasterize-3d rasterize-lib cairo-util)

set_target_properties(rasterize-3d rasterize-lib
    PROPERTIES
    FOLDER "construction/3d-model-rasterization"
)

add_subdirectory(test)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)