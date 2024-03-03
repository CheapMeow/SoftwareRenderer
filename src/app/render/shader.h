#ifndef SHADER_H
#define SHADER_H

#include "math/matrix.h"
#include "math/vector3.h"
#include <array>

struct IShader
{
    virtual ~IShader() {};
    virtual Vector3 vertex(Vector3& vertex, Matrix4& MVP, Vector3& normals, Vector3& light, int i) = 0;
    virtual bool    fragment(Vector3& bari, Vector3& color, float& depth, Vector3& zVerts)         = 0;
};

struct FlatShader : public IShader
{
    float   varIntensity;
    Vector3 rgb {255, 255, 255};

    Vector3 vertex(Vector3& vertex, Matrix4& MVP, Vector3& normals, Vector3& light, int index) override
    {
        varIntensity = std::max(0.0f, normals.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(Vector3& bari, Vector3& color, float& depth, Vector3& zVerts) override
    {
        depth = bari.dotProduct(zVerts);
        color = rgb * varIntensity;
        // color.print();
        return false;
    }
};

struct GouraudShader : public IShader
{
    Vector3 varying_intensity;
    Vector3 rgb {255, 255, 255};

    Vector3 vertex(Vector3& vertex, Matrix4& MVP, Vector3& normals, Vector3& light, int index) override
    {
        // normals.print();
        varying_intensity.data[index] = std::max(0.0f, normals.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(Vector3& bari, Vector3& color, float& depth, Vector3& zVerts) override
    {
        // varying_intensity.print();
        float intensity = bari.y * varying_intensity.x + bari.z * varying_intensity.y + bari.x * varying_intensity.z;
        color           = rgb * intensity;
        depth           = bari.dotProduct(zVerts);
        return false;
    }
};

#endif