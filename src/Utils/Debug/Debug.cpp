#include "Debug.h"
#include <iostream>

void GetError() {
    GLenum error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            std::cout << "LOG::GL_NO_ERROR No error was found" << std::endl;
            break;
        case GL_INVALID_ENUM:
            std::cout << "ERROR::GL_INVALID_ENUM An unacceptable value is specified for an enumerated argument" << std::endl; 
            break; 
        case GL_INVALID_VALUE:
            std::cout << "ERROR::GL_INVALID_VALUE A numeric argument is out of range" << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cout << "ERROR::GL_INVALID_OPERATION The specified operation is not allowed in the current state" << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "ERROR::GL_INVALID_FRAMEBUFFER_OPERATION The framebuffer object is not complete" << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "ERROR::GL_OUT_OF_MEMORY There is not enough memory left to execute the command" << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "ERROR::GL_STACK_UNDERFLOW An attempt has been made to perform an operation that would cause an internal stack to underflow" << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "ERROR::GL_STACK_UNDERFLOW An attempt has been made to perform an operation that would cause an internal stack to overflow" << std::endl;
            break;
    }
}
