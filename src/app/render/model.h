#ifndef MODEL_H
#define MODEL_H

#include "math/matrix.h"
#include "mesh.h"
#include "string"


class Model
{

public:
    Model(std::string path);
    void  describeMesh();
    Mesh* getMesh();
    void  initPosition(TransformParameters initPos);

private:
    void buildMesh(std::string path);
    void loadVertices(std::ifstream& fileHandle);
    void loadFaces(std::ifstream& fileHandle);
    Mesh mMesh;
};

#endif