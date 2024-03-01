#include "rasterizer.h"
#include "vector"

const SDL_PixelFormat* Rasterizer::mappingFormat(SDL_AllocFormat(PIXEL_FORMAT));

void Rasterizer::makeCoolPattern()
{
    Uint32 red   = SDL_MapRGBA(mappingFormat, 0xFF, 0x00, 0x00, 0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00, 0xFF, 0x00, 0xFF);
    Uint32 blue  = SDL_MapRGBA(mappingFormat, 0x00, 0x00, 0xFF, 0xFF);

    for (int y = 0; y < mCanvas->mHeight; ++y)
    {
        for (int x = 0; x < mCanvas->mWidth; ++x)
        {
            Uint8  r     = x * y % 256;
            Uint8  g     = y % 256;
            Uint8  b     = x % 256;
            Uint32 color = SDL_MapRGBA(mappingFormat, r, g, b, 0xFF);
            setPixelColor(color, x, y);
        }
    }
}

void Rasterizer::drawModels(Model* models)
{
    Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF);
    Uint32 red   = SDL_MapRGBA(mappingFormat, 0xFF, 0x00, 0x00, 0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00, 0xFF, 0x00, 0xFF);
    for (Vector3 f : (models->getMesh()).faces)
    {
        Vector3 v1 = (models->getMesh()).vertices[f.x - 1];
        Vector3 v2 = (models->getMesh()).vertices[f.y - 1];
        Vector3 v3 = (models->getMesh()).vertices[f.z - 1];
        drawLine(v1, v2, white);
        drawLine(v2, v3, red);
        drawLine(v1, v3, green);
    }
}

void Rasterizer::drawLine(Vector3 vertex1, Vector3 vertex2, Uint32 color)
{
    int scalingFactor = 65;
    int x1            = (vertex1.x * scalingFactor) + mCanvas->mWidth * 1 / 2;
    int y1            = (-vertex1.y * scalingFactor) + mCanvas->mHeight * 2 / 3;
    int x2            = (vertex2.x * scalingFactor) + mCanvas->mWidth * 1 / 2;
    int y2            = (-vertex2.y * scalingFactor) + mCanvas->mHeight * 2 / 3;

    for (float t = 0.; t < 1.; t += .11)
    {
        int xf = x1 * (1. - t) + x2 * t;
        int yf = y1 * (1. - t) + y2 * t;
        if ((xf >= 0) && (yf >= 0) && (xf < mCanvas->mWidth) && (yf < mCanvas->mHeight))
        {
            setPixelColor(color, xf, yf);
        }
    }
}

void Rasterizer::setPixelColor(Uint32 color, int x, int y)
{
    int arrayCoordinates               = convertCoordinates(x, y);
    mCanvas->mBuffer[arrayCoordinates] = color;
}

int Rasterizer::convertCoordinates(int x, int y) { return ((y * mCanvas->mWidth) + x); }
