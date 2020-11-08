#include <fstream>
#include <sstream>
#include "MeshLoader.h"

Mesh MeshLoader::loadFromFile(const std::string& filePath) {
    std::ifstream meshFile;
    // ensure ifstream objects can throw exceptions:
    meshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        meshFile.open(filePath);
        std::stringstream meshStream;
        // read file's buffer contents into streams
        Mesh mesh = createMeshFromStream(meshStream);
        // close file handlers
        meshFile.close();

        return mesh;
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Filestream ERROR [ Shader file not successfully read | Exception: " << e.what() << " ]" << std::endl;
        // streams must always be closed
        if (meshFile.is_open()) {
            meshFile.close();
        }

        // Mesh failed to load so we stop the engine
        exit(EXIT_FAILURE);
    }
}

std::vector<Vec4> MeshLoader::parseVertex(std::stringstream& sin) {

}

std::vector<Vec3> MeshLoader::parseNormal(std::stringstream& sin) {

}

std::vector<Vec2> MeshLoader::parseTextCoord(std::stringstream& sin) {

}