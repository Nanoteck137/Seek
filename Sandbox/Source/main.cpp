#include "SandboxApp.h"

int main(int argc, char** argv)
{
    // TODO(patrik): Move this to a Engine Initialize method or something like
    // that
    SK_PROFILE_BEGIN_SESSION("Startup", "Seek-Startup.json");
    Seek::Log::Init();

    SandboxApp* app = new SandboxApp();
    SK_PROFILE_END_SESSION();

    SK_PROFILE_BEGIN_SESSION("Runtime", "Seek-Runtime.json");
    app->Run();
    SK_PROFILE_END_SESSION();

    delete app;

    return 0;
}