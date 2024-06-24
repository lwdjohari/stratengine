cmake_minimum_required(VERSION 3.10)
project(stratengine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(STRATENGINE_VER "0.0.9")
message( STATUS "Stratengine v${STRATENGINE_VER}")
message( STATUS "-----------------------------")

option(BUILD_SHARED_LIBS OFF)
# Assimp
option(ASSIMP_INSTALL OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INJECT_DEBUG_POSTFIX ON)
# GLM
option(GLM_BUILD_INSTALL OFF)
option(GLM_ENABLE_CXX_17 ON)
option(GLM_ENABLE_FAST_MATH ON)
option(GLM_BUILD_TESTS OFF)

include(ProjectCXX)
set(ISROOT FALSE)
set(CXX_FEATURE "")
NV_DETECT_IS_ROOT_PROJECT(ISROOT)

if(ISROOT)
  if(NOT STX_CXX_VERSION)
    set(STX_CXX_VERSION 17)
  endif()
    option(STX_CXX_STANDARD_REQUIRED "CXX Required" ON)
    option(STX_CXX_EXTENSIONS "CXX Extensions" OFF)
  NV_PROPOGATE_CXX_OPTIONS(${STX_CXX_VERSION} ${STX_CXX_STANDARD_REQUIRED} ${STX_CXX_EXTENSIONS})
  set(STX_USE_CATCH ON)
  set(STX_USE_TEST ON)
  set(STX_BUILD_EXAMPLE ON)
  set(CXX_FEATURE "")
else()
    set(STX_CXX_VERSION ${CMAKE_CXX_STANDARD})
endif()

message(STATUS "")

message(STATUS "Stratengine LIB DIR:${CMAKE_CURRENT_SOURCE_DIR}")


include(ProjectCXX)
set(ISROOT FALSE)
NV_DETECT_IS_ROOT_PROJECT(ISROOT)
message(STATUS "Stratengine Root Project: ${ISROOT} ${STX_CXX_VERSION}")

NV_GET_CXX_STD_FEATURE(${STX_CXX_VERSION} CXX_FEATURE)
message(STATUS "CXX Feature: ${CXX_FEATURE}")

find_package(Threads REQUIRED)

set(SDL_SHARED OFF)
# # Add SDL2
message(STATUS "SDL2: ON")
add_subdirectory(thirdparty/sdl build-sdl)
message(STATUS "SDL2 Include DIR: ${SDL2_INCLUDE_DIRS}")


# Add bgfx
set(BGFX_INSTALL OFF)
message(STATUS "bgfx: ON")
add_subdirectory(thirdparty/bgfx-cmake build-bgfx)
message(STATUS "bgfx Include DIR: ${BGFX_DIR}/include")
message(STATUS "bgimg Include DIR: ${BIMG_DIR}/include")
message(STATUS "bx Include DIR: ${BX_DIR}/include")




set(GLM_BUILD_INSTALL OFF)
set(GLM_ENABLE_CXX_17 ON)
set(GLM_ENABLE_FAST_MATH ON)
set(GLM_BUILD_TESTS OFF)
add_subdirectory(thirdparty/glm build-glm)

#add Assimp
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX ON)
add_subdirectory(thirdparty/assimp build-assimp)


# Include directories


#Main headers and sources
file(GLOB_RECURSE SOURCES_STX CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/*.s
)


add_library(${PROJECT_NAME} STATIC ${SOURCES_STX} )
target_link_libraries(${PROJECT_NAME}  
    PUBLIC
        SDL2-static
        bgfx
        bx
        glm::glm
        assimp
    )
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${PROJECT_NAME} PUBLIC "STRATENGINE_USE_LIBUV=0")
target_compile_features(${PROJECT_NAME} PUBLIC ${CXX_FEATURE})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fp16/
        ${BGFX_DIR}/include
        ${BIMG_DIR}/include
        ${BX_DIR}/include
)

add_library(${PROJECT_NAME}::stratengine ALIAS ${PROJECT_NAME} )


#runner executable
add_executable(${PROJECT_NAME}_runner engine/main.cc)
target_link_libraries(${PROJECT_NAME}_runner PUBLIC stratengine::stratengine)
set_target_properties(${PROJECT_NAME}_runner PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME}_runner PUBLIC ${CXX_FEATURE})

