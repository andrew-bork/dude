#pragma once

// #include <memory>
#include <shader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include <memory>

class Geometry {
    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indexes;
        std::vector<float> color;
        // float * vertices;
        
        unsigned int _vertexBufferObject = -1;
        unsigned int _indexBufferObject = -1;
        unsigned int _colorBufferObject = -1;

        Geometry();
        void generateBuffers();
        // Geometry(const std::vector<float>& _vertices);
        ~Geometry();
};

class Material {
    public:
        unsigned int _handle = -1;
        Material();
        Material(const Shader& _vertex, const Shader& _fragment);
        void setShaders(const Shader& _vertex, const Shader& _fragment);
        ~Material();

        unsigned int getAttributeLocation(const char * attribute);
        unsigned int getUniformLocation(const char * uniform);
};

class Mesh {
    public:
        std::shared_ptr<Geometry> geometry;
        std::shared_ptr<Material> material;

        glm::vec4 position;
        // glm::vec4 scale;

        Mesh(std::shared_ptr<Geometry> _geometry, std::shared_ptr<Material> _material);
        ~Mesh();

        void drawMesh();
};
