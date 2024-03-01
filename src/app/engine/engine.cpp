#include "engine.h"
#include "math/matrix.h"
#include "math/vector3.h"

#include <stdio.h>
#include <string>

Engine::Engine() {}

Engine::~Engine() {}

bool Engine::startUp()
{
    bool success = true;
    // Startup window manager and create window
    if (!FEWindowManager.startUp())
    {
        success = false;
        printf("Failed to initialize window manager.\n");
    }
    else
    {
        if (!FERenderManager.startUp(FEWindowManager))
        {
            success = false;
            printf("Failed to initialize render manager.\n");
        }
        else
        {
            if (!FEInputManager.startUp())
            {
                success = false;
                printf("Failed to initialize input manager.\n");
            }
        }
    }
    return success;
}

void Engine::shutDown()
{
    delete sceneModels;
    sceneModels = nullptr;
    FEInputManager.shutDown();
    FERenderManager.shutDown();
    FEWindowManager.shutDown();
}

void Engine::mainLoop()
{
    bool         done  = false;
    int          count = 0;
    unsigned int end   = 0;
    unsigned int start = 0;
    printf("Entered Main Loop!\n");

    while (!done)
    {
        start = SDL_GetTicks();
        ++count;

        // Handle all user input
        done = FEInputManager.processInput();

        // Update entities here in the future
        // Right now only does simple demo stuff
        // Maybe physics based in the future??
        int dt = start - end;
        moveModels(dt);

        // Perform all render calculations and update screen
        FERenderManager.render(sceneModels);
        end = SDL_GetTicks();
        // SDL_Delay(100);
        printf("%2.1d: Loop elapsed time (ms):%d\n", count, end - start);
    }
}

void Engine::loadModels()
{
    // In the future I want to read all o the models in the model folder
    // And build them here.  For now I force it to be only one.
    // Probably should be its own class in the future
    std::string path = "../../../resources/models/teapot.obj";
    sceneModels      = new Model(path);

    // sceneModels->describeMesh();
}

// Engine class moves stuff, for now
// Some kind of physics module should be responsible of this in the future
// Actually it should probably be moved by user input
void Engine::moveModels(int dt)
{

    // Creating model matrix
    Vector3 position    = Vector3(0, 0, 0);
    Vector3 rotation    = Vector3(0.01, 0.01, 0.01);
    Vector3 scaling     = Vector3(1, 1, 1);
    Matrix4 modelMatrix = Matrix4::modelMatrix(position, rotation, scaling);

    // Getting vector of vertices
    Mesh*                 modelMesh = sceneModels->getMesh();
    int                   size      = modelMesh->numVertices;
    std::vector<Vector3>* vertices  = &modelMesh->vertices;

    // Applying the multiplication
    for (int i = 0; i < size; ++i)
    {
        (*vertices)[i] = modelMatrix.matMultVec((*vertices)[i], 1);
    }
}