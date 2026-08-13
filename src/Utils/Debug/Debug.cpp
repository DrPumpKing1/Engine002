#include "Debug.h"
#include "../Logger/Logger.h"

void GetError() {
    GLenum error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            LOG("LOG::GL_NO_ERROR No error was found");
            break;
        case GL_INVALID_ENUM:
            LOG("ERROR::GL_INVALID_ENUM An unacceptable value is specified for an enumerated argument");
            break; 
        case GL_INVALID_VALUE:
            LOG("ERROR::GL_INVALID_VALUE A numeric argument is out of range");
            break;
        case GL_INVALID_OPERATION:
            LOG("ERROR::GL_INVALID_OPERATION The specified operation is not allowed in the current state");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOG("ERROR::GL_INVALID_FRAMEBUFFER_OPERATION The framebuffer object is not complete");
            break;
        case GL_OUT_OF_MEMORY:
            LOG("ERROR::GL_OUT_OF_MEMORY There is not enough memory left to execute the command");
            break;
        case GL_STACK_UNDERFLOW:
            LOG("ERROR::GL_STACK_UNDERFLOW An attempt has been made to perform an operation that would cause an internal stack to underflow");
            break;
        case GL_STACK_OVERFLOW:
            LOG("ERROR::GL_STACK_UNDERFLOW An attempt has been made to perform an operation that would cause an internal stack to overflow");
            break;
    }
}
