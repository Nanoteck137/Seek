# TODO

## Refactoring
* Notes
  - Add a UI Layer
  - Application Singleton
  - Common place to initialize Engine subsystems
  - Rethink the Audio system
  - Instrumentor Singleton
  - Find a solution to the Texture Class enums and structs
  - Seperate the FileSystem api

* App
  - Input
  - Window
* Audio
  - AudioEngine
  - Sound
* Physics
  - PhysicsBox2D
  - PhysicsWorld2D
* System
  - FileSystem
* UI
  * Components
    - UIBlock
    - UIButton
    - UIContainer
    - UIText
  * Constraints
    - UICenterConstraint
    - UIPixelConstraint
    - UIRatioConstraint
    - UIRelativeConstraint
    - UITextHeightConstraint
  * Transition
    - Animator
    - Modifier
    - SlideDriver
    - SlideTransition
    - Transition
    - TransitionType
    - ValueDriver
    - ValueTransition
  * Other
    - UIComponent
    - UIConstraint
    - UIConstriants
    - UIManager
* Other
  - Log

## Engine
* Audio (XAudio2, OpenAL?)
* FileSystem
  - Virtual FileSystem (VFS)
  - File Infomation
  - Directory Infomation
    - File Change Callback???
* Color Class (Change all glm::vec4)

## Renderering Engine
* UI 
  - Cleanup
  - More Components
    - Slider
    - TextBox
  - Fix Memory issues
  - More transitions
  - Fix text scaling
* 2D
* 3D
* General
  - Framebuffers/RenderTargets

## Physics Engine
* Box2D
  - More Shapes
  - Collision Callback
  - Collisions Masks
  - Triggers/Sensors