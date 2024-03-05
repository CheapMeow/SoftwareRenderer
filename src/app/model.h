#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include "matrix.h"
#include "mesh.h"
#include "texture.h"
#include <string>


class Model
{
public:
    Model(std::string path, TransformParameters& initParameters);

    Mesh*    getMesh();
    Matrix4* getModelMatrix();
    AABox*   getBounds();
    Texture* getAlbedo();
    Texture* getNormal();

    void update();

    // Prints the mesh vertices for debugging
    void describeMesh();

private:
    Texture mAlbedo {"../../../resources/models/fire_hydrant_Base_Color.png", "RGB"};
    Texture mNormal {"../../../resources/models/fire_hydrant_Normal_OpenGL.png", "XYZ"};
    Mesh    mMesh;
    AABox   mBounds;
    Matrix4 mModelMatrix;
};

#endif