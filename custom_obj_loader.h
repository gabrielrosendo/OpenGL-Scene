#ifndef CUSTOM_OBJ_LOADER_H
#define CUSTOM_OBJ_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Function to load a basic OBJ file (vertices and normals)
bool loadOBJ(const char* path, std::vector<float>& vertices, std::vector<float>& normals) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<float> temp_vertices;
    std::vector<float> temp_normals;
    std::vector<unsigned int> vertexIndices, normalIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string word;
        s >> word;

        if (word == "v") {
            float x, y, z;
            s >> x >> y >> z;
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
        }
        else if (word == "vn") {
            float x, y, z;
            s >> x >> y >> z;
            temp_normals.push_back(x);
            temp_normals.push_back(y);
            temp_normals.push_back(z);
        }
        else if (word == "f") {
            std::string vertexStr;
            while (s >> vertexStr) {
                unsigned int vertexIndex = 0, normalIndex = 0;
                size_t firstSlash = vertexStr.find('/');
                size_t secondSlash = vertexStr.find('/', firstSlash + 1);

                if (firstSlash == std::string::npos) {
                    // Only vertex index
                    vertexIndex = std::stoi(vertexStr);
                } else if (secondSlash == std::string::npos || firstSlash == secondSlash) {
                    // Format: v//vn (no texture coordinate)
                    vertexIndex = std::stoi(vertexStr.substr(0, firstSlash));
                    normalIndex = std::stoi(vertexStr.substr(firstSlash + 2));
                } else {
                    // Format: v/vt/vn
                    vertexIndex = std::stoi(vertexStr.substr(0, firstSlash));
                    normalIndex = std::stoi(vertexStr.substr(secondSlash + 1));
                }

                if (vertexIndex > 0 && vertexIndex <= temp_vertices.size() / 3) {
                    vertexIndices.push_back(vertexIndex - 1);
                }

                if (normalIndex > 0 && normalIndex <= temp_normals.size() / 3) {
                    normalIndices.push_back(normalIndex - 1);
                }
            }
        }
    }

    // Assemble the vertex and normal arrays
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];

        vertices.push_back(temp_vertices[vertexIndex * 3]);
        vertices.push_back(temp_vertices[vertexIndex * 3 + 1]);
        vertices.push_back(temp_vertices[vertexIndex * 3 + 2]);

        normals.push_back(temp_normals[normalIndex * 3]);
        normals.push_back(temp_normals[normalIndex * 3 + 1]);
        normals.push_back(temp_normals[normalIndex * 3 + 2]);
    }

    return true;
}

#endif

