#include "BuilderApp.h"

#include "GameLayer.h"

BuilderApp::BuilderApp() { PushLayer(new GameLayer()); }

BuilderApp::~BuilderApp() {}
