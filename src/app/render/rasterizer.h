#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "SDL.h"
#include "buffer.h"
#include "math/vector3.h"
#include "shader.h"

class Rasterizer
{

public:
    static void drawTriangles(Vector3* vertices, IShader& shader, Buffer<Uint32>* pixelBuffer, Buffer<float>* zBuffer);

    static void drawWireFrame(Vector3* vertices, IShader& shader, Buffer<Uint32>* pixelBuffer);

    static void testPattern(Buffer<Uint32>* pixelBuffer);

    static void makeCoolPattern(Buffer<Uint32>* pixelBuffer);

    static void drawLine(Vector3& vertex1, Vector3& vertex2, const Uint32& color, Buffer<Uint32>* pixelBuffer);

    static void screenSpaceTransform(Buffer<Uint32>*     pixelBuffer,
                                     Vector3*            vertices,
                                     std::array<int, 3>& xV,
                                     std::array<int, 3>& yV,
                                     Vector3&            zV);

    // Don't look at my ugly code!
    static void
    baricentric(Vector3& lambdas, float InvArea, int x, int y, std::array<int, 3>& xV, std::array<int, 3>& yV);

private:
    Rasterizer() {};

    // Setting this equal to the same pixel format our textures are in
    static const Uint32           PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA8888;
    static const SDL_PixelFormat* mappingFormat;

    // Some basic colors
    static const Uint32 white;
    static const Uint32 red;
    static const Uint32 green;
    static const Uint32 blue;
};

#endif