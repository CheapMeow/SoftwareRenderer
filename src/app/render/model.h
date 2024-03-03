#ifndef MODEL_H
#define MODEL_H

#include "math/bound3.h"
#include "math/matrix.h"
#include "mesh.h"
#include "objParser.h"
#include "string"


class Model
{
public:
    Model(std::string path);
    void describeMesh();

    Mesh* getMesh();

    void initPosition(TransformParameters initPos);
    void update();

private:
    void   buildBoundaryBox();
    Mesh   mMesh;
    Bound3 mBounds;
};

#endif