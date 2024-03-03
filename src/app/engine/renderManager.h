#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "displayManager.h"
#include "render/mesh.h"
#include "sceneManager.h"
#include "softwareRenderer.h"
#include <queue>

// High level render operations that shouldn't be done by the
// basic graphics lib.
class RenderManager
{

public:
    // Dummy constructors / Destructors
    RenderManager();
    ~RenderManager();

    // Gets scene and display info to
    bool startUp(DisplayManager& displayManager, SceneManager& sceneManager);
    void shutDown();

    // Performs all high level prep operations that the graphics library
    // Needs to do before acting
    void render();

private:
    void buildRenderQueue();
    bool initSoftwareRenderer();

    // This is a pointer to a pointer to allow for scene switching
    SceneManager*     sceneLocator;
    DisplayManager*   screen;
    SoftwareRenderer  renderInstance;
    std::queue<Mesh*> renderQueue;
};

#endif