# Bubble Bobble (Custom Game Engine)

[![GitHub Repo](https://img.shields.io/badge/GitHub-Repository-blue?logo=github)](https://github.com/CasperVanLaerHowest/Programming-4-BubbleBobble-Caspervlaer)

A custom C++ 2D game engine built on top of Minigin using **SDL3** and **GLM**, developed for the Programming 4 course at Howest DAE. This engine recreates the classic arcade game **Bubble Bobble**, implementing clean architectural design patterns to separate game logic, rendering, audio, and physics.

## Core Features & Gameplay
* **Classic Bubble Bobble Mechanics**: Bubble shooting, jumping, floating physics, enemy entrapment, popping mechanics, and falling states.
* **Maita Enemy Variant**: Advanced enemy behavior capable of moving and firing projectile balls towards players.
* **Game Modes**: Support for Single Player, Co-op, and Versus mode.
* **Data-driven Levels**: Levels, platforms, and game modes are fully parsed and loaded dynamically from text configurations (`Levels.txt`).
* **Highscore System**: Local highscore persistence (saving/sorting player names and scores in `Data/HighScores.txt`).
* **Audio System**: Event-driven sounds for shooting, hit detection, pickups, level changes, and music.

## Architectural Design Patterns
To keep the engine modular, performant, and extensible, the following design choices were implemented:

1. **Component Pattern**
   * Game objects are structured using a component-based model (`Component` base class). 
   * Separation of concerns: rendering, movement, states, and game logic are separated into discrete, reusable components such as `PhysicsComponent`, `CollisionComponent`, `BubbleStateComponent`, `EnemyStateComponent`, and `HighScoreEntryComponent`.

2. **Command Pattern**
   * Input handling is completely decoupled from actor logic using the Command Pattern.
   * Player/system commands (e.g., `MoveCommand`, `ShootBubbleCommand`, `SkipLevelCommand`, `HighScoreInputCommand`) map input keys/buttons dynamically to component methods.
   * Supports both keyboard scans and Xbox controller triggers.

3. **State Pattern**
   * Used to model complex, state-dependent behavior for players, bubbles, and enemies.
   * Example: Enemies transition between standard walking, bubble-entrapped floating, angry/accelerated mode, and dying. This prevents nested conditional checks and ensures clean state transitions.

4. **Observer Pattern & Event Queue**
   * Decoupled communication using a centralized `GameEventQueue`.
   * Broadcasters publish events (e.g., score points, player hurt, level transition), which are received by registered listeners.
   * `AudioObserver` listens to events to play corresponding sound effects without tight coupling to game physics or player code.

5. **Service Locator**
   * Used for the Audio engine hook (`AudioLocator`) to allow components to request audio playback globally without knowing the concrete sound library details.

---

# Use

Get the source from this project, or since students need to have their work on github too, they can use this repository as a template. Hit the "Use this template" button on the top right corner of the github page of this project.

## Windows version

Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake 
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.

Or
- Use whatever editor you like :)

## Emscripten (web) version

### On windows

For installing all of the needed tools on Windows I recommend using [Chocolatey](https://chocolatey.org/). You can then run the following in a terminal to install what is needed:

    choco install -y cmake
    choco install -y emscripten
    choco install -y ninja
    choco install -y python

In a terminal, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.

### On OSX

On Mac you can use homebrew

    brew install cmake
    brew install emscripten
    brew install python

In a terminal on OSX, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake .. -DCMAKE_OSX_ARCHITECTURES=""
    emmake make

To be able to see the webpage you can start a python webserver in the build_web folder

    python3 -m http.server

Then browse to http://localhost:8000 and you're good to go.

## Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://avadae.github.io/minigin/). 
  - The url of that page will be `https://<username>.github.io/<repository>/`
- You can embed this page with 

```<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://<username>.github.io/<repository>/" loading="lazy"></iframe>```

