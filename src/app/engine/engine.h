#ifndef ENGINE_H
#define ENGINE_H

// Headers
#include "inputManager.h"
#include "render/model.h"
#include "renderManager.h"
#include "windowManager.h"

class Engine
{

public:
    Engine();

    ~Engine();

    bool startUp();

    void shutDown();

    void mainLoop();

    void loadModels();

    void moveModels();

private:
    WindowManager FEWindowManager;
    RenderManager FERenderManager;
    InputManager  FEInputManager;
    Model*        sceneModels;
};

#endif