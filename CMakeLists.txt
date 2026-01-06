cmake_minimum_required(VERSION 3.14)
project(Loom)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8))
    message(FATAL_ERROR "This project requires 64-bit Windows (x64)")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_compile_definitions(NOMINMAX)

if (MSVC)
    add_compile_options(/EHsc)
    add_compile_options(/D_ITERATOR_DEBUG_LEVEL=0)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
# set(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_compile_definitions(
    _ITERATOR_DEBUG_LEVEL=0
    _HAS_ITERATOR_DEBUGGING=0
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
        TLS_VERIFY ON
    )
endif ()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage("gh:BasedInc/libhat@0.9.0")
CPMAddPackage("gh:cursey/safetyhook@0.6.9")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage("gh:skypjack/entt#fe8d7d78c4823e8a66a050bf86f5c6318cf76ce7")
CPMAddPackage("gh:woidptr/kawa_meta#19f188ceda374714ac788e52f94e7f663b1cf43d")
CPMAddPackage(
    URI "gh:BasedInc/kiero#269860311ac04b0ee89e68f68bca55abd02a3390"
    OPTIONS
        "KIERO_INCLUDE_D3D11 ON"
        "KIERO_INCLUDE_D3D12 ON"
        "KIERO_BUILD_EXAMPLES OFF"
)

target_link_libraries(${PROJECT_NAME} PRIVATE libhat::libhat safetyhook::safetyhook kiero::kiero EnTT::EnTT nlohmann_json kawa_meta::kawa_meta)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.5
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    add_library(imgui)

    target_sources(imgui PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp

        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    )

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )

    target_link_libraries(imgui PUBLIC d3d11 d3d12 dxgi)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

function(ADD_RESOURCES out_var)
    set(result)
    foreach (in_f ${ARGN})
        file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
        set(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
        add_custom_command(OUTPUT ${out_f}
            COMMAND ld.exe -r -b binary -o ${out_f} ${src_f}
            DEPENDS ${in_f}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Building object ${out_f}"
            VERBATIM
        )
        list(APPEND result ${out_f})
    endforeach()
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

file(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} CONFIGURE_DEPENDS "assets/*")
add_resources(EMBEDDED_RESOURCES ${RESOURCES})
add_resources(SIGNATURES signatures.json)

target_sources(${PROJECT_NAME} PRIVATE ${EMBEDDED_RESOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${SIGNATURES})
