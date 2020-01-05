#include "SeekPCH.h"
#include <Seek.h>

#include <imgui.h>

class TestLayer : public Seek::Layer
{
public:
	TestLayer()
		: Layer("Test")
	{
	}

	void OnUpdate(Seek::Timestep ts) override
	{
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Seek::Event &event) override
	{
		// SK_APP_TRACE("'{0}' got event '{1}'", GetName(), event);
	}
};

class SandboxApp : public Seek::Application
{
public:
	SandboxApp()
	{
		PushLayer(new TestLayer());
	}

	~SandboxApp()
	{
	}
};

int main(int argc, char **argv)
{
	Seek::Log::Init();

	SandboxApp *app = new SandboxApp();
	app->Run();
	delete app;

	return 0;
}