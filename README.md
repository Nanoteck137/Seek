# Seek

## TODO

### Refactoring
* App
  - Application
  - Input
  - KeyCodes
  - Layer
  - LayerStack
  - MouseButtonCodes
  - Timestep
  - Window
* Audio
  - AudioEngine
  - Sound
* Debug
  - Instrumentor
* Events
  - ApplicationEvent
  - Event
  - KeyEvents
  - MouseEvents
* ImGui
  - ImGuiLayer
* Physics
  - PhysicsBox2D
  - PhysicsWorld2D
* Renderer
  - Buffer
  - Font
  - FontManager
  - GraphicsContext
  - OrthograpicCamera
  - RenderCommand
  - Renderer
  - Renderer2D
  - RendererAPI
  - Shader
  - Texture
  - VertexArray
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
  - Core
  - Log
  - Types

### Engine
* Audio (XAudio2, OpenAL?)
* FileSystem
  - Virtual FileSystem (VFS)
  - File Infomation
  - Directory Infomation
    - File Change Callback???
* Color Class (Change all glm::vec4)

### Renderering Engine
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

### Physics Engine
* Box2D
  - More Shapes
  - Collision Callback
  - Collisions Masks
  - Triggers/Sensors

### Notes

Engine Classes
```c++
// .h File
#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Bar.h"

namespace Seek 
{
    class Foo : public Bar
    {
    // Constructor
    public:
    protected:
    private:
        Foo();
        ~Foo();

    public:
        // Virtual Methods
        // Normal Methods
        // Static Methods
    protected:
        // Virtual Methods
        // Normal Methods
        // Static Methods
    private:
        // Virtual Methods
        // Normal Methods
        // Static Methods

    public:
        // Normal Variables
        // Static Variables
    protected:
        // Normal Variables
        // Static Variables
    private:
      // Normal Variables
      // Static Variables
    };
}

// .cpp File
#include "SeekPCH.h"
#include "Seek/Foo.h"

namespace Seek 
{
    // Static Variables initializtion

    // Constructor

    // Virtual Methods

    // Normal Methods

    // Static Methods
}
```