#include <shader.h>
// #include <SDL3/SDL_opengl.h>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

static int vertexShaderCount = 0, fragmentShaderCount = 0;
Shader::Shader(ShaderType _type) {
    type = _type;
    switch(type){
    case vertex:
        vertexShaderCount++;
        shaderName = "Vertex Shader #" + std::to_string(vertexShaderCount);
        _handle = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "Created Vertex Shader \"" << shaderName << "\"" << std::endl;
        break;
    case fragment:
        fragmentShaderCount++;
        shaderName = "Fragment Shader #" + std::to_string(fragmentShaderCount);
        _handle = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "Created Fragment Shader \"" << shaderName << "\"" << std::endl;
        break;
    } 
}

Shader::Shader(std::string _shaderName, ShaderType _type) {
    type = _type;
    switch(type){
    case vertex:
        vertexShaderCount++;
        shaderName = _shaderName;
        _handle = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "Created Vertex Shader \"" << shaderName << "\"" << std::endl;
        break;
    case fragment:
        fragmentShaderCount++;
        shaderName = _shaderName;
        _handle = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "Created Fragment Shader \"" << shaderName << "\"" << std::endl;
        break;
    } 
}

Shader::~Shader() {
    std::cout << "Deleting Shader \"" << shaderName << "\"" << std::endl;
    glDeleteShader(_handle);
}

void Shader::loadFromString(std::string _source) {
    // Convert to cstring and send to shader
    source = _source;
    const char* tmp = source.c_str();
    glShaderSource(_handle, 1, &tmp, NULL);

    // Compile shader
    glCompileShader(_handle);
    
    // Checks if compilation encountered an error (throws a runtime exception)
    int  success;
    char infoLog[512];
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(_handle, 512, NULL, infoLog);
        throw std::runtime_error("Failed to Compile Shader \""+shaderName+"\""+infoLog);
    }
}
void Shader::loadFromFile(std::string filename) {
    // Load from file
    std::ifstream f(filename);
    std::stringstream buffer;
    buffer << f.rdbuf();
    source = buffer.str();

    // Convert to cstring and send to shader
    const char* tmp = source.c_str();
    glShaderSource(_handle, 1, &tmp, NULL);
    
    // Compile shader
    glCompileShader(_handle);

    // Checks if compilation encountered an error (throws a runtime exception)
    int  success;
    char infoLog[512];
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(_handle, 512, NULL, infoLog);
        throw std::runtime_error("Failed to Compile Shader \""+shaderName+"\""+infoLog);
    }
}