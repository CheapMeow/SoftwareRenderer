#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SDL.h"
#include "render/canvas.h"
#include "render/model.h"
#include "render/rasterizer.h"
#include "render/texture.h"
#include "windowManager.h"

class RenderManager
{

public:
    RenderManager();

    ~RenderManager();

    bool startUp(WindowManager windowManager);

    void render(Model* models, Matrix4& mat);

    void shutDown();

private:
    // Init methods
    bool createRenderer(SDL_Window* mainWindow);
    bool createCanvas();
    bool createScreenTexture();
    void createProjectionMatrix();

    // Rendering pipeline stuff
    void clearScreen();
    void updateScreen();

    // Per vertex stuff

    // Culling
    bool frustrumCulling(Model* model, Matrix4& viewMatrix);

    SDL_Renderer* mainRenderer;
    Texture       screenTexture;
    Canvas*       mainCanvas;
    Rasterizer*   raster;
    Matrix4       projectionMatrix;
};

#endif