#include <mesh.h>
#include <glad/glad.h>

#include <iostream>

Geometry::Geometry() {


}
// Geometry::Geometry(const std::vector<float>& _vertices) {
//     vertices = _vertices;
//     glGenBuffers(1, &_vbo);
//     glGenVertexArrays(1, &_vao);

//     glBindVertexArray(_vao);

//     glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
    
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * vertices.size(), (void*)0);
//     glEnableVertexAttribArray(0);
// }

void Geometry::generateBuffers() {
    if(_indexBufferObject == -1) glGenBuffers(1,&_indexBufferObject);
	if(_vertexBufferObject == -1) glGenBuffers(1,&_vertexBufferObject);
	if(_colorBufferObject == -1) glGenBuffers(1,&_colorBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexes.size(), &indexes[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	//glBindAttribLocation(program,1,"colorBuffer");
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color.size(), &color[0], GL_STATIC_DRAW);

}

Geometry::~Geometry() {

}

Material::Material() {}

Material::Material(const Shader& _vertex, const Shader& _fragment) {
    _handle = glCreateProgram();
    glAttachShader(_handle, _vertex._handle);
    glAttachShader(_handle, _fragment._handle);
    glLinkProgram(_handle);

    char infoLog[512];
    int success = 0;
    glGetProgramiv(_handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_handle, 512, NULL, infoLog);
        throw std::runtime_error("Failed to link: " + std::string(infoLog));
    }
}

void Material::setShaders(const Shader& _vertex, const Shader& _fragment) {
    _handle = glCreateProgram();
    glAttachShader(_handle, _vertex._handle);
    glAttachShader(_handle, _fragment._handle);
    
    glLinkProgram(_handle);

    char infoLog[512];
    int success = 0;
    glGetProgramiv(_handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_handle, 512, NULL, infoLog);
        throw std::runtime_error("Failed to link: " + std::string(infoLog));
    }

    glDetachShader(_handle, _vertex._handle);
    glDetachShader(_handle, _fragment._handle);
}

Material::~Material() {
    glDeleteProgram(_handle);
    std::cout << "deleting material" << std::endl;
}

unsigned int Material::getAttributeLocation(const char * attribute) {
    return glGetAttribLocation(_handle, attribute);
}
unsigned int Material::getUniformLocation(const char * uniform) {
    return glGetUniformLocation(_handle, uniform);
}

Mesh::Mesh(std::shared_ptr<Geometry> _geometry, std::shared_ptr<Material> _material) {
    geometry = _geometry;
    material = _material;
}

Mesh::~Mesh() {
    std::cout << "deleting mesh" << std::endl;
}
void Mesh::drawMesh() {
    glUseProgram(material->_handle);
    // glBindVertexArray(geometry->_vao);
    glDrawArrays(GL_TRIANGLES, 0, geometry->vertices.size()/3);
}