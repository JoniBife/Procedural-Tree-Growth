#include <GL/glew.h> // include glew to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shaderProgram.h"

// creates and compiles a shader 
static unsigned int compileShader(GLenum type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Returns a pointer to the first character of the string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        int logLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = new char[logLength]();
        glGetShaderInfoLog(id, logLength, &logLength, infoLog);

        std::string shaderType = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
        std::cout << "ERROR::SHADER_COMPILATION_ERROR_" << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        delete[] infoLog;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

/* creates a shader program and attaches and links the vertexShader and fragmentShader
 * deletes the shaders if they are attached and linked successfully
 */
static unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int programme = glCreateProgram();

    glAttachShader(programme, vertexShader);
    glAttachShader(programme, fragmentShader);
    glLinkProgram(programme);

    int success;
    glGetProgramiv(programme, GL_LINK_STATUS, &success);

    if (!success) {
        int logLength;
        glGetProgramiv(programme, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = new char[logLength]();
        glGetProgramInfoLog(programme, logLength, &logLength, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;

        glDeleteProgram(programme);
        return 0;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDetachShader(programme, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(programme, fragmentShader);
    glDeleteShader(fragmentShader);

    return programme;
}

// returns the content of the files in the vertexShaderPath and fragmentShaderPath
static bool readShaderFiles(
    const char* vShaderPath,
    const char* fShaderPath,
    std::string& vShaderCode,
    std::string& fShaderCode) {

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vShaderPath);
        fShaderFile.open(fShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vShaderCode = vShaderStream.str();
        fShaderCode = fShaderStream.str();

        return true;
    }
    catch (std::ifstream::failure& e)
    {
        // TODO Add error message
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;

        // streams must always be closed
        if (vShaderFile.is_open()) {
            vShaderFile.close();
        }
        if (fShaderFile.is_open()) {
            fShaderFile.close();
        }

        return false;
    }
}

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexShaderCode;
        std::string fragmentShaderCode;
        readShaderFiles(vertexShaderPath, fragmentShaderPath, vertexShaderCode, fragmentShaderCode);

        // 2. compile shaders
        // vertex shader
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode);
        // fragment shader
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
        // shader Program
        programID = createShaderProgram(vertexShader, fragmentShader);
    }
// activate the shader
void ShaderProgram::use()
{
    glUseProgram(programID);
}
// utility uniform functions
void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}




