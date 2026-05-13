if(EMSCRIPTEN)
  set(TARGET_NAME BubbleBobble_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME BubbleBobble)
  set(TARGET_PARAM WIN32)
endif()

add_executable(${TARGET_NAME} ${TARGET_PARAM}
  Main.cpp
  Commands/TakeDamageCommand.cpp
  Commands/TakeDamageCommand.h
  Commands/AddScoreCommand.cpp
  Commands/AddScoreCommand.h
  Commands/MoveCommand.cpp
  Commands/MoveCommand.h
  Components/ImGuiComponent.cpp 
  Components/ImGuiComponent.h
  Components/PlayerFacingComponent.cpp
  Components/PlayerFacingComponent.h
  Components/FPSComponent.cpp
  Components/FPSComponent.h
  Components/RotatorComponent.cpp
  Components/RotatorComponent.h
  Components/HealthComponent.cpp
  Components/HealthComponent.h
  Components/ScoreComponent.cpp
  Components/ScoreComponent.h
  Components/TextComponent.cpp
  Components/TextComponent.h
  Observers/HealthObserver.cpp
  Observers/HealthObserver.h
  Observers/ScoreObserver.cpp
  Observers/ScoreObserver.h
  Observers/SteamWinObserver.cpp
  Observers/SteamWinObserver.h
  Components/PhysicsComponent.h
  Components/PhysicsComponent.cpp "Components/CollisionComponent.h" "Components/CollisionComponent.cpp" "States/BaseState.h" "Components/PlayerStateComponent.h" "Components/PlayerStateComponent.cpp" "States/IdleState.h" "Components/AnimationComponent.h" "Components/AnimationComponent.cpp" "States/IdleState.cpp" "States/WalkState.h" "States/WalkState.cpp" "Commands/JumpCommand.h" "Commands/JumpCommand.cpp" "HelperFunctions.h" "HelperFunctions.cpp" "Commands/ShootBubbleCommand.h" "Commands/ShootBubbleCommand.cpp")

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

target_link_libraries(${TARGET_NAME} PRIVATE Minigin)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
endif()

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
  set(SDL_RUNTIME_DLLS
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
  )

  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(
      TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:${dllTarget}>"
          "$<TARGET_FILE_DIR:${TARGET_NAME}>"
      VERBATIM
    )
  endforeach()

  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PRIVATE Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>")
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )

    # Copy SDL3 DLLs
    add_custom_command(TARGET BubbleBobble POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"             # Gets the SDL3.dll
        "$<TARGET_FILE_DIR:BubbleBobble>"
    )

    # Copy SDL3_ttf DLLs
    add_custom_command(TARGET BubbleBobble POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"     # Gets the SDL3_ttf.dll
        "$<TARGET_FILE_DIR:BubbleBobble>"
    )

    # Copy SDL3_mixer DLLs
    add_custom_command(TARGET BubbleBobble POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>" # Gets the SDL3_mixer.dll
        "$<TARGET_FILE_DIR:BubbleBobble>"
    )
endif()
