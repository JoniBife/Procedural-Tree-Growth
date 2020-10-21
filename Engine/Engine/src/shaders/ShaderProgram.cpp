#include "ShaderProgram.h"

#define VERTICES 0
#define COLORS 1

// In the future we should add other constructors to support other types of shaders
ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& fragmentShader) : vertexShader(vertexShader), fragmentShader(fragmentShader) 
{
	id = glCreateProgram();

    // Attaching shaders to program
    glAttachShader(id, vertexShader.getId());
    glAttachShader(id, fragmentShader.getId());
    
    // This step is unnecessary if you use the location specifier in your shader
    // e.g. layout (location = 0) in vec3 in_Position;
    glBindAttribLocation(id, VERTICES, "in_Position");
    glBindAttribLocation(id, COLORS, "in_Color");

	glLinkProgram(id);

    GLint programLinked;
    glGetProgramiv(id, GL_LINK_STATUS, &programLinked);

    if (programLinked != GL_TRUE) {
        int logLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = new char[logLength]();
        glGetProgramInfoLog(id, logLength, &logLength, infoLog);
        std::cout << "Program linking ERROR [ " << infoLog << " ]" << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        glDeleteProgram(id);
        exit(EXIT_FAILURE);
    }

    // Program has been created and successfully linked so we detach the shaders
    glDetachShader(id, vertexShader.getId());
    glDetachShader(id, fragmentShader.getId());
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::use() {
    glUseProgram(id);
}
void ShaderProgram::stopUsing() {
    glUseProgram(0);
}

GLuint ShaderProgram::getProgramId() const {
    return id;
}

// All theses methods se the value of a specific uniform
void ShaderProgram::setUniform(const GLint location, const GLint value) {
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const GLint location, const GLfloat value) {
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const GLint location, const Vec2& value) {
    float array[2];
    value.toOpenGLFormat(array);
    glUniform2fv(location, 1, array);
}
void ShaderProgram::setUniform(const GLint location, const Vec3& value) {
    float array[3];
    value.toOpenGLFormat(array);
    glUniform3fv(location, 1, array);
}
void ShaderProgram::setUniform(const GLint location, const Vec4& value) {
    float array[4];
    value.toOpenGLFormat(array);
    glUniform4fv(location, 1, array);
}
void ShaderProgram::setUniform(const GLint location, const Mat2& value) {
    float array[4];
    value.toOpenGLFormat(array);
    glUniformMatrix2fv(location, 1, GL_FALSE, array);
}
void ShaderProgram::setUniform(const GLint location, const Mat3& value) {
    float array[9];
    value.toOpenGLFormat(array);
    glUniformMatrix3fv(location, 1, GL_FALSE, array);
}
void ShaderProgram::setUniform(const GLint location, const Mat4& value) {
    float array[16];
    value.toOpenGLFormat(array);
    glUniformMatrix4fv(location, 1, GL_FALSE, array);
}

// Used to obtain the location of uniforms, this way we avoid doing this in every setUniform
GLint ShaderProgram::getUniformLocation(const GLchar* name) const {
    return glGetUniformLocation(id, name);
}

// Output, prints the id
std::ostream& operator<<(std::ostream& os, const ShaderProgram& sp) {
    // TODO
    return os;
}