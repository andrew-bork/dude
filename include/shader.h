#pragma once
#include <string>

class Shader {
    public:
        enum ShaderType {
            vertex=0, fragment=1, none=-1
        };
        ShaderType type = ShaderType::none;
        std::string source = "";
        std::string shaderName = "";
        unsigned int _handle = -1;
        // Shader();
        Shader();
        Shader(std::string _shaderName);
        ~Shader();
        void loadFromString(std::string _source, ShaderType type);
        void loadFromFile(std::string filename, ShaderType type);
};