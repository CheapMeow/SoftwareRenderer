#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "mesh.h"
#include <fstream>
#include <sstream>


namespace OBJ
{
    Mesh& buildMeshFromFile(Mesh& mesh, std::string& path);

    bool fileExists(std::string& path);

    // Get vertices and Normal data
    void loadFileData(Mesh& mesh, std::ifstream& file);

    std::vector<std::string> split(std::string& str, char delim);

} // namespace OBJ

#endif
