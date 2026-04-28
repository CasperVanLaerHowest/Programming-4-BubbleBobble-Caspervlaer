# We create Minigin as a STATIC library
add_library(Minigin STATIC
  Component.h
  Command.h
  ControllerInput.cpp
  ControllerInput.h
  Font.cpp
  Font.h
  GameObject.cpp
  GameObject.h
  GameTime.h
  InputManager.cpp
  InputManager.h
  Minigin.cpp
  Minigin.h
  Observer.h
  RenderComponent.h
  Renderer.cpp
  Renderer.h
  ResourceManager.cpp
  ResourceManager.h
  Scene.cpp
  Scene.h
  SceneManager.cpp
  SceneManager.h
  Subject.h
  Texture2D.cpp
  TextureComponent.h
  Transform.cpp
  TransformComponent.cpp
  TransformComponent.h
)

# Enable C++20
target_compile_features(Minigin PUBLIC cxx_std_20)

target_include_directories(Minigin PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${glm_SOURCE_DIR}
  ${sdl3_SOURCE_DIR}/include
  ${sdl3_ttf_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  ${VLD_INCLUDE_DIR}
)

# Link dependencies to the engine library
target_link_libraries(Minigin PUBLIC
  SDL3::SDL3
  SDL3_ttf::SDL3_ttf
  glm::glm
  imgui
  ${VLD_LIBRARIES}
)

if(WIN32)
  target_link_libraries(Minigin PUBLIC Xinput)
endif()

if(MSVC)
  target_compile_options(Minigin PRIVATE /W4 /WX)
else()
  target_compile_options(Minigin PRIVATE -Wall -Wextra -Werror)
endif()