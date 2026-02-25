cmake_minimum_required(VERSION 3.14)
project(Loom)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8))
    message(FATAL_ERROR "This project requires 64-bit Windows (x64)")
endif()

set(SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
set(GENERATED_IMPORTS "${CMAKE_CURRENT_BINARY_DIR}/import.def")
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(SIGNATURES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/signatures.json")

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_command(
    OUTPUT ${GENERATED_IMPORTS}
    COMMAND "${Python3_EXECUTABLE}" "${SCRIPTS_DIR}/funcs.py" "${SIGNATURES_FILE}" "${GENERATED_IMPORTS}"
    DEPENDS "${SCRIPTS_DIR}/funcs.py"
    COMMENT "Generating sdk functions"
    VERBATIM
)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.hpp" "${CMAKE_CURRENT_BINARY_DIR}/Resource.cpp"
    COMMAND "${Python3_EXECUTABLE}" "${SCRIPTS_DIR}/embedder.py" "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.hpp" "${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.cpp"
    DEPENDS "${SCRIPTS_DIR}/embedder.py" "${ASSETS_DIR}"
    COMMENT "Embedding resources"
    VERBATIM
)

add_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    NOKANJI
    NOHELP
)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.cpp" "${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/embedder")

set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedder/Resource.cpp" PROPERTIES GENERATED TRUE)

set(IS_MSVC "$<CXX_COMPILER_ID:MSVC>")
set(IS_CLANG "$<CXX_COMPILER_ID:Clang>")
set(IS_RELEASE "$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>")

target_compile_options(${PROJECT_NAME} PRIVATE
    # Disable rtti
    $<$<BOOL:${IS_MSVC}>:/GR->
    $<$<BOOL:${IS_CLANG}>:/clang:-fno-rtti>

    # MSVC
    # Disable exceptions for release
    $<$<AND:${IS_MSVC},${IS_RELEASE}>:/EHs-c->
    $<$<AND:${IS_MSVC},$<NOT:${IS_RELEASE}>>:/EHsc>

    # Clang / clang-cl / GCC
    # Disable exceptions for release
    $<$<AND:${IS_CLANG},${IS_RELEASE}>:/clang:-fno-exceptions>

    # Enable #embed directive support in clang
    $<$<BOOL:${IS_CLANG}>:/clang:-Wno-c23-extensions>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:_HAS_EXCEPTIONS=0>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:_HAS_EXCEPTIONS=0>)

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
CPMAddPackage("gh:ocornut/imgui@1.92.5")
CPMAddPackage("gh:cursey/safetyhook@0.6.9")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage("gh:simdjson/simdjson@4.2.4")
CPMAddPackage("gh:skypjack/entt#fe8d7d78c4823e8a66a050bf86f5c6318cf76ce7")
CPMAddPackage("gh:superPuero/kawa_core#a04c1012a9e32bb1472d2fc92fcf2efb51b84793")
# CPMAddPackage("gh:corrosion-rs/corrosion@0.6.0")
CPMAddPackage(
    URI "gh:BasedInc/kiero#269860311ac04b0ee89e68f68bca55abd02a3390"
    OPTIONS
        "KIERO_INCLUDE_D3D11 ON"
        "KIERO_INCLUDE_D3D12 ON"
        "KIERO_BUILD_EXAMPLES OFF"
)

if (NOT TARGET imgui)
    add_library(imgui STATIC)

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
endif ()

#[[ corrosion_import_crate(
    MANIFEST_PATH ${CMAKE_SOURCE_DIR}/utils/linker/Cargo.toml
)
corrosion_set_env_vars(linker "RUST_ASSETS_DIR=${ASSETS_DIR}")
corrosion_set_env_vars(linker "RUST_SIGNATURES_FILE=${SIGNATURES_FILE}") ]]

target_link_libraries(${PROJECT_NAME} PRIVATE libhat::libhat safetyhook::safetyhook kiero::kiero EnTT::EnTT kawa_core imgui Dbghelp nlohmann_json simdjson) # linker

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mcapi.lib
    COMMAND lib /DEF:${GENERATED_IMPORTS} /OUT:${CMAKE_CURRENT_BINARY_DIR}/mcapi.lib /MACHINE:x64
    DEPENDS ${GENERATED_IMPORTS}
)

add_custom_target(RefLibGen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mcapi.lib)
add_library(RefLib UNKNOWN IMPORTED)
set_property(TARGET RefLib PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/mcapi.lib)
add_dependencies(RefLib RefLibGen)

target_link_libraries(${PROJECT_NAME} PRIVATE RefLib)

target_link_options(${PROJECT_NAME} PRIVATE "/DELAYLOAD:mcapi.dll")
