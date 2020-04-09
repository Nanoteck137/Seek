#pragma once

// --------------- CORE ---------------
#include "Seek/Core.h"
#include "Seek/Log.h"

// --------------- SYSTEM ---------------
#include "Seek/Debug/Instrumentor.h"

#include "Seek/Application.h"
#include "Seek/Input.h"

#include "Seek/Timestep.h"

// TODO(patrik): Move layers?
#include "Seek/Layer.h"
#include "Seek/LayerStack.h"

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

// --------------- PHYSICS 2D ---------------
#include "Seek/Physics/PhysicsWorld2D.h"
#include "Seek/Physics/PhysicsBox2D.h"

// --------------- AUDIO ---------------
#include "Seek/Audio/AudioEngine.h"
#include "Seek/Audio/Sound.h"

// --------------- SYSTEM ---------------
#include "Seek/System/FileSystem.h"
