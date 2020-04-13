#pragma once

// --------------- CORE ---------------
#include "Seek/Core.h"
#include "Seek/Log.h"

// --------------- DEBUG ---------------
#include "Seek/Debug/Instrumentor.h"

// --------------- DEBUG ---------------
#include "Seek/App/Application.h"
#include "Seek/App/Input.h"
#include "Seek/App/KeyCodes.h"
#include "Seek/App/Layer.h"
#include "Seek/App/LayerStack.h"
#include "Seek/App/MouseButtonCodes.h"
#include "Seek/App/Timestep.h"
#include "Seek/App/Window.h"

// --------------- EVENTS ---------------
#include "Seek/Events/Event.h"
#include "Seek/Events/ApplicationEvent.h"
#include "Seek/Events/KeyEvents.h"
#include "Seek/Events/MouseEvent.h"

// --------------- IMGUI ---------------
#include "Seek/ImGui/ImGuiLayer.h"

// --------------- RENDERER ---------------
#include "Seek/Renderer/Renderer.h"
#include "Seek/Renderer/Renderer2D.h"
#include "Seek/Renderer/Texture.h"
#include "Seek/Renderer/Shader.h"
#include "Seek/Renderer/OrthographicCamera.h"

#include "Seek/Renderer/Font.h"
#include "Seek/Renderer/FontManager.h"

#include "Seek/Renderer/TextureManager.h"

// ------------------ UI-------------------
#include "Seek/UI/Components/UIBlock.h"
#include "Seek/UI/Components/UIButton.h"
#include "Seek/UI/Components/UIContainer.h"
#include "Seek/UI/Components/UIText.h"

#include "Seek/UI/Constraints/UICenterConstraint.h"
#include "Seek/UI/Constraints/UIPixelConstraint.h"
#include "Seek/UI/Constraints/UIRatioConstraint.h"
#include "Seek/UI/Constraints/UIRelativeConstraint.h"
#include "Seek/UI/Constraints/UITextHeightConstraint.h"

#include "Seek/UI/Transition/Animator.h"
#include "Seek/UI/Transition/Modifier.h"
#include "Seek/UI/Transition/SlideDriver.h"
#include "Seek/UI/Transition/SlideTransition.h"
#include "Seek/UI/Transition/Transition.h"
#include "Seek/UI/Transition/TransitionType.h"
#include "Seek/UI/Transition/ValueDriver.h"
#include "Seek/UI/Transition/ValueTransition.h"

#include "Seek/UI/UIComponent.h"
#include "Seek/UI/UIConstraint.h"
#include "Seek/UI/UIConstraints.h"
#include "Seek/UI/UIManager.h"

// --------------- PHYSICS 2D ---------------
#include "Seek/Physics/PhysicsWorld2D.h"
#include "Seek/Physics/PhysicsBox2D.h"

// --------------- AUDIO ---------------
#include "Seek/Audio/AudioEngine.h"
#include "Seek/Audio/Sound.h"

// --------------- SYSTEM ---------------
#include "Seek/System/FileSystem.h"
