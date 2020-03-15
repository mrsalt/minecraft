#enable_testing()

include_directories(
    .
    model-readers
    model-writers
)

add_executable(rasterize-3d
    main.cpp
    ModelBuilder.cpp
    ModelGenerator.cpp
    model-readers/ModelReader_Wavefront.cpp
    model-writers/ModelWriter_Wavefront.cpp
)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)