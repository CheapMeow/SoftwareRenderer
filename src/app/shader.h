#ifndef SHADER_H
#define SHADER_H

#include "vector3D.h"
#include "matrix.h"
#include "texture.h"
#include <array>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3f vertex(Vector3f &vertex, Vector3f &normals, Vector3f &textureVals, Vector3f &light, int i) = 0;
    virtual Vector3f fragment(float u, float v) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    Matrix4 MVP, MV;
    float varIntensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normals, Vector3f &textureVals, Vector3f &light, int index) override{
        varIntensity = std::max(0.0f,normals.dotProduct(light));
        return MVP.matMultVec(vertex); //Transforms verts into projected space
    }

    Vector3f fragment(float u, float v) override{
        return rgb*varIntensity;
    }

};

//More Complex shader that calculates a per-vertex intensity and interpolates 
//through the fragments of the triangle
struct GouraudShader : public IShader {
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.5, spec = 0;
    Vector3f ambient, diffuse, specular;
    Vector3f lightColor1{1,1,1}, lightColor2{0,0,1}, lightColor3{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, viewDir, light2;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &textureVals, Vector3f &light, int index) override{
        normal = N.matMultDir(normal).normalized();
        light2 = V.matMultDir(light).normalized();
        reflectDir = Vector3f::reflect(-light2, normal);
        viewDir = MV.matMultVec(vertex).normalized();
        varying_specular.data[index] = std::pow( std::max( -viewDir.dotProduct(reflectDir), 0.0f), 32.0f);
        varying_diffuse.data[index] = std::max(0.0f, (normal).dotProduct(-light2));
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        ambient = lightColor1 * ambientStrength;

        diffStrength = varying_diffuse.x + u*(varying_diffuse.y - varying_diffuse.x) + v*(varying_diffuse.z - varying_diffuse.x);
        diffuse = lightColor2 * diffStrength;

        spec = varying_specular.x + u*(varying_specular.y - varying_specular.x) + v*(varying_specular.z - varying_specular.x);
        specular = lightColor3 * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
    }

};

//Even more complex shader that interpolates normals and calculates intensities per fragment instead
//instead of per vertex.
struct PhongShader : public IShader {
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.9, spec = 0;
    Vector3f normals[3], viewDir[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir;
    Vector3f lightColor{0,0.1,1},lightColorSpec{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, light2;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &textureVals, Vector3f &light, int index) override{
        normals[index] = N.matMultDir(normal).normalized();
        viewDir[index] = MV.matMultVec(vertex).normalized();
        light2 = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpNormal = normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v;
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diffStrength = std::max(0.0f, (interpNormal.normalized()).dotProduct(light2));
        diffuse = lightColor * diffStrength;
        
        //Specular
        reflectDir = Vector3f::reflect(-light2, interpNormal);
        spec = std::pow( std::max( (-interpViewDir.normalized()).dotProduct(reflectDir), 0.0f), 50.0f);
        specular = lightColorSpec * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
    }

};

//Optimized version of Phong shader that uses a half angle instead of individual reflection
//angles
struct BlinnPhongShader : public IShader {
    Texture *albedo;
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength=1 , specularStrength= 0.5;
    float diff, spec, shininess = 128;
    Vector3f normals[3], viewDir[3], UV[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir, interpUV;
    Vector3f lightColor{1,1,1};
    Vector3f halfwayDir, lightDir;
    Vector3f interpCol, white{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &textureVals, Vector3f &light, int index) override{
        normals[index] = N.matMultDir(normal).normalized();
        UV[index] = textureVals;
        viewDir[index] = MV.matMultVec(vertex).normalized();
        lightDir = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpNormal = (normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v).normalized();
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        interpUV = UV[0] + (UV[1] - UV[0])* u + (UV[2] - UV[0]) * v;
        //Albedo
        interpCol = albedo->getPixelVal(interpUV.x, interpUV.y);
        //rgb.print();

        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diff = std::max(0.0f, interpNormal.dotProduct(lightDir));
        diffuse = lightColor * diff * diffStrength;
        
        //Specular
        halfwayDir = (lightDir - interpViewDir).normalized();
        spec = std::pow(std::max(0.0f, interpNormal.dotProduct(halfwayDir)), shininess);
        specular = lightColor * spec * specularStrength;

        return (ambient + diffuse) * interpCol + specular * white;
    }

};


struct TestShader{
    Texture *albedo;
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength=1 , specularStrength= 0.5;
    float diff, spec, shininess = 128;
    Vector3f normals[3], viewDir[3], UV[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir, interpUV;
    Vector3f lightColor{1,1,1};
    Vector3f halfwayDir, lightDir;
    Vector3f interpCol, white{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &textureVals, Vector3f &light, int index) {
        normals[index] = N.matMultDir(normal).normalized();
        UV[index] = textureVals;
        viewDir[index] = MV.matMultVec(vertex).normalized();
        lightDir = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) {
        //Interpolated stuff
        interpNormal = (normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v).normalized();
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        interpUV = UV[0] + (UV[1] - UV[0])* u + (UV[2] - UV[0]) * v;
        //Albedo
        interpCol = albedo->getPixelVal(interpUV.x, interpUV.y);
        //rgb.print();

        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diff = std::max(0.0f, interpNormal.dotProduct(lightDir));
        diffuse = lightColor * diff * diffStrength;
        
        //Specular
        halfwayDir = (lightDir - interpViewDir).normalized();
        spec = std::pow(std::max(0.0f, interpNormal.dotProduct(halfwayDir)), shininess);
        specular = lightColor * spec * specularStrength;

        return (ambient + diffuse) * interpCol + specular * white;
    }

};


#endif