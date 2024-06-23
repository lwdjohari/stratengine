cmake_minimum_required(VERSION 3.10)
project(stratengine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(STRATENGINE_VER "0.0.9")
message( STATUS "Stratengine v${STRATENGINE_VER}")
message( STATUS "-----------------------------")

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
    )
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${PROJECT_NAME} PUBLIC "STRATENGINE_USE_LIBUV=0")
target_compile_features(${PROJECT_NAME} PUBLIC ${CXX_FEATURE})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fp16/
        ${BGFX_DIR}/include
        ${BIMG_DIR}/include
)

add_library(${PROJECT_NAME}::stratengine ALIAS ${PROJECT_NAME} )


#runner executable
add_executable(${PROJECT_NAME}_runner engine/main.cc)
target_link_libraries(${PROJECT_NAME}_runner PUBLIC stratengine::stratengine)
set_target_properties(${PROJECT_NAME}_runner PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME}_runner PUBLIC ${CXX_FEATURE})

