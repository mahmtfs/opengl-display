cmake_minimum_required(VERSION 3.0)
project(openg_display)

set(OS_BITS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OS_BITS 64)
endif()

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OS_BITS}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/static")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

add_subdirectory(libs/nanogui)
include_directories(libs/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

include_directories(libs/glm)
add_subdirectory(libs/glm)

set(INCLUDES_DIR "includes")
include_directories(${INCLUDES_DIR})

add_executable(${PROJECT_NAME} main.cpp 
                ${INCLUDES_DIR}/graphics/graphics.cpp
                ${INCLUDES_DIR}/shader/shader.cpp
                ${INCLUDES_DIR}/importer/importer.cpp)
target_link_libraries(${PROJECT_NAME} nanogui ${NANOGUI_EXTRA_LIBS})

add_custom_target(copy_shaders ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/shaders
                ${OUTPUT_DIR}/shaders
                COMMENT "Copying shaders...") 

add_custom_target(copy_models ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/models
                ${OUTPUT_DIR}/models
                COMMENT "Copying models...") 

add_custom_target(copy_textures ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/textures
                ${OUTPUT_DIR}/textures
                COMMENT "Copying textures...")

add_dependencies(${PROJECT_NAME} copy_shaders)
add_dependencies(${PROJECT_NAME} copy_models)
add_dependencies(${PROJECT_NAME} copy_textures)