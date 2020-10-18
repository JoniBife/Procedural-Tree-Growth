/*Using this preprocessor directives informs the compiler to only include and 
compile this header file if it hasn't been included yet, even if multiple 
files include the shader header. This prevents linking conflicts.*/
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>
#include <string>

class ShaderProgram
{
public:
    // the program ID
    unsigned int programID;

    // constructor reads and builds the shader
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};
#endif
