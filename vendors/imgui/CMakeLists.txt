project(imgui LANGUAGES NONE)

file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx12.cpp"
)

add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends"
)

target_link_libraries(imgui PUBLIC d3d11 d3d12 dxgi)

add_library(ImGui::ImGui ALIAS imgui)