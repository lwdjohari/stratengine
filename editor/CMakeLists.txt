cmake_minimum_required(VERSION 3.10)
project(strate)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(STRATE_VERSION_HEADER "${CMAKE_SOURCE_DIR}/version.h")

set(STRATENGINE_VER "0.0.9")
message( STATUS "Stratengine Editor v${STRATENGINE_VER}")
message( STATUS "-----------------------------")


NV_GET_CXX_STD_FEATURE(${STX_CXX_VERSION} CXX_FEATURE)
message(STATUS "CXX Feature: ${CXX_FEATURE}")



#Main headers and sources
file(GLOB_RECURSE SOURCES_STX_EDITOR CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.s
)


add_library(${PROJECT_NAME} STATIC ${SOURCES_STX_EDITOR} )
target_link_libraries(${PROJECT_NAME}  
    PUBLIC
        fmt
        GPUOpen::VulkanMemoryAllocator
        vk-bootstrap::vk-bootstrap
        SDL2-static
        imgui
        glfw
        glm::glm
        assimp
        meshoptimizer
        fastgltf
        EnTT::EnTT
        stratengine::stratengine
    )
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${PROJECT_NAME} PUBLIC "STRATENGINE_USE_LIBUV=0")
target_compile_features(${PROJECT_NAME} PUBLIC ${CXX_FEATURE})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fp16/
)

add_library(${PROJECT_NAME}::strate ALIAS ${PROJECT_NAME} )


#runner executable
add_executable(${PROJECT_NAME}-editor main.cc)
target_link_libraries(${PROJECT_NAME}-editor PUBLIC strate::strate)
set_target_properties(${PROJECT_NAME}-editor PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME}-editor PUBLIC ${CXX_FEATURE})

