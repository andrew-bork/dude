#include <shader.h>
// #include <SDL3/SDL_opengl.h>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

static int vertexShaderCount = 0, fragmentShaderCount = 0;
Shader::Shader() {
    type = none;
}

Shader::Shader(std::string _shaderName)  {
    type = none;
    shaderName = _shaderName;
}

Shader::~Shader() {
    std::cout << "Deleting Shader \"" << shaderName << "\"" << std::endl;
    glDeleteShader(_handle);
}

GLuint shaderTypeLookup[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

void Shader::loadFromString(std::string _source, ShaderType _type) {
    if(_type != type) {
        glDeleteShader(_handle);
        _handle = -1;
    }
    if(_handle == -1) {
        type = _type;
        _handle = glCreateShader(shaderTypeLookup[(int) type]);
    }

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
void Shader::loadFromFile(std::string filename, ShaderType _type) {
    if(_type != type) {
        glDeleteShader(_handle);
        _handle = -1;
    }
    if(_handle == -1) {
        type = _type;
        _handle = glCreateShader(shaderTypeLookup[(int) type]);
    }

    // Load from file
    std::ifstream f(filename);
    if(!f.is_open()) throw std::runtime_error("Could not open \""+filename+"\". Maybe i don't have permissions or the file doesnt exist");
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