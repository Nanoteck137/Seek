#include "SandboxApp.h"

#include "TestLayer.h"

SandboxApp::SandboxApp() { PushLayer(new TestLayer()); }

SandboxApp::~SandboxApp() {}