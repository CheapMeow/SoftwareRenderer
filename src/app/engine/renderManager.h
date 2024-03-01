#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SDL.h"
#include "render/canvas.h"
#include "render/rasterizer.h"
#include "structs/texture.h"
#include "windowManager.h"


class RenderManager
{

public:
    RenderManager();

    ~RenderManager();

    bool startUp(WindowManager windowManager);

    bool createRenderer(SDL_Window* mainWindow);

    bool createScreenTexture();

    void clearScreen();

    void updateScreen();

    bool createCanvas();

    void render();

    void shutDown();

private:
    SDL_Renderer* mainRenderer;
    Texture       screenTexture;
    Canvas*       mainCanvas;
    Rasterizer*   raster;
};

#endif