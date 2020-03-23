#enable_testing()

include_directories(
    .
    generation
    geometry
    model-readers
    model-writers
    utils
)

add_executable(rasterize-3d
    main.cpp
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
)

add_subdirectory(test)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)