#include "SandboxApp.h"

int main(int argc, char** argv)
{
    SK_PROFILE_BEGIN_SESSION("Startup", "Seek-Startup.json");

    // TODO(patrik): Move this to a Engine Initialize method or something like
    // that
    Seek::Log::Init();

    Seek::AudioEngine::Init();

    SandboxApp* app = new SandboxApp();
    SK_PROFILE_END_SESSION();

    SK_PROFILE_BEGIN_SESSION("Runtime", "Seek-Runtime.json");
    app->Run();
    SK_PROFILE_END_SESSION();

    delete app;

    Seek::AudioEngine::Shutdown();

    return 0;
}