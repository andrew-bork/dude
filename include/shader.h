#pragma once
#include <string>

class Shader {
    public:
        enum ShaderType {
            vertex, fragment
        };
        ShaderType type;
        std::string source = "";
        std::string shaderName = "";
        unsigned int _handle = -1;
        Shader(ShaderType _type);
        Shader(std::string _shaderName, ShaderType _type);
        ~Shader();
        void loadFromString(std::string _source);
        void loadFromFile(std::string filename);
};