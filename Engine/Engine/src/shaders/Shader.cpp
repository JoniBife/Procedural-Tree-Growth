#include "Shader.h"

// Receives the shader file path and compiles the shader
Shader(const GLenum shaderType, const std::string& filePath);
// Deletes the shader using glDeleteShader
~Shader();

// Getters
std::string getFilePath() const;
GLuint getId() const;
GLenum getType() const;
std::string getCode() const;

// Output, prints the id, the type, the file path and the code
friend std::ostream& operator<<(std::ostream& os, const Shader& s);


// Obtains the shader code from the filePath. Returns false if an error occurred while trying to read the file
bool readShaderFromFile(const std::string& filePath, std::string& shaderCode);

// Compiles the shader and returns false if compilation failed
bool compileShader();