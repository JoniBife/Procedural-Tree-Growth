#ifndef OPEN_GL_UTILS_H
#define OPEN_GL_UTILS_H

#include <iostream>

const std::string errorString(GLenum error);

/*
* Checks for any OpenGL errors and prints an
* error message for each error.
* 
* After all errors it prints the error parameter.
* 
* Closes the program if an error occurs.
*/
void checkForOpenGLErrors(std::string error);


#define ASSERT(x) if (!(x)) __debugbreak();
#if _DEBUG
#define glCall(x) clearError();\
	x;\
	ASSERT(hasOpenGLError(#x,__FILE__, __LINE__))

static void clearError() { while (glGetError() != GL_NO_ERROR); }
static bool hasOpenGLError(const char* function, const char* file, int line) {
	while (GLenum errCode = glGetError()) {
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
		return true;
	}
	return false;
}
#else
#define glCall(x) x;
#endif

#endif
