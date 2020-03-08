#enable_testing()

add_executable(rasterize-3d main.cpp model-readers/ModelReader_CS5400.cpp)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)