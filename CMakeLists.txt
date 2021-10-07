cmake_minimum_required(VERSION 3.1)
project(rayTracer)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
set(SOURCES 
"${SRC_DIR}/Source.cpp" 
"${SRC_DIR}/initScene.cpp" 
"${SRC_DIR}/objImporter.cpp" 
)
set(HEADERS 
"${SRC_DIR}/initScene.hpp" 
"${SRC_DIR}/objImporter.hpp" 
"${SRC_DIR}/light.hpp" 
"${SRC_DIR}/object.hpp" 
"${SRC_DIR}/ray.hpp" 
"${SRC_DIR}/progressbar.hpp" 
"${SRC_DIR}/stb_image.h" 
"${SRC_DIR}/scene.hpp" 
"${SRC_DIR}/texture.hpp" 
"${SRC_DIR}/imageTexture.hpp" 
)

file( GLOB MODELS 
"${SRC_DIR}/Models/*.obj" 
)

file(COPY ${MODELS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Models")
file(COPY ${MODELS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/Models")

include_directories("${LIB_DIR}/glm")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)