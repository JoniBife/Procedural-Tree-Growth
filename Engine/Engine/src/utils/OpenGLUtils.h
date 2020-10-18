#ifndef OPEN_GL_UTILS_H
#define OPEN_GL_UTILS_H

#include <iostream>

/*
* Checks for any OpenGL errors and prints an
* error message for each error.
* 
* After all errors it prints the error parameter.
* 
* Closes the program if an error occurs.
*/
static void checkForOpenGLErrors(std::string error);

#endif
