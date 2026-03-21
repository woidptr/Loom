option(USE_IMGUI "Enable imgui" ON)
option(USE_GAMEINPUT "Enable GameInput v2" ON)

if(USE_IMGUI)
    add_subdirectory(imgui)
endif()

if(USE_GAMEINPUT)
    add_subdirectory(game_input)
endif()