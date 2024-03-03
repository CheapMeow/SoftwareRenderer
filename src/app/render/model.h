#ifndef MODEL_H
#define MODEL_H

#include "math/matrix.h"
#include "mesh.h"
#include "objParser.h"
#include <string>

class Model
{
public:
    Model(std::string path, TransformParameters& initParameters);

    Mesh* getMesh();

    void update();

    // Prints the mesh vertices for debugging
    void describeMesh();

private:
    // Transform matrix from object space to worldSpace
    void initPosition(TransformParameters initPos);

    // Calculates the boundary box of the mesh in object space
    void   buildBoundaryBox();
    Mesh   mMesh;
    Bound3 mBounds;
};

#endif