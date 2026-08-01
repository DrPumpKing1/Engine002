CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
CMAKE_POLICY(SET CMP0072 NEW)

PROJECT(Engine002 VERSION 0.0.1)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS ON)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")

FILE(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(glm CONFIG REQUIRED)
FIND_PACKAGE(assimp CONFIG REQUIRED)

ADD_LIBRARY(GLAD src/gl.c)
TARGET_INCLUDE_DIRECTORIES(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/includes)
ADD_LIBRARY(STB_IMAGE src/stb_image.cpp)
TARGET_INCLUDE_DIRECTORIES(STB_IMAGE PRIVATE ${CMAKE_SOURCE_DIR}/includes)

SET(FILE_READER
    "src/Utils/FileReader/FileReader.h"
    "src/Utils/FileReader/FileReader.cpp"
)

SET(DEBUG_SRC
    "src/Utils/Debug/Debug.h"
    "src/Utils/Debug/Debug.cpp"
)

SET(UTILS 
    ${FILE_READER}
    ${DEBUG_SRC}
    "src/Utils/Utils.h"
)

SET(SHADER
    "src/Engine/Shader/Shader.h"
    "src/Engine/Shader/Shader.cpp"
    "src/Engine/Shader/ShaderProgram.h"
    "src/Engine/Shader/ShaderProgram.cpp"
    "src/Engine/Shader/ShaderProgramUniforms.cpp"
    "src/Engine/Shader/ShaderUniforms.h"
    "src/Engine/Shader/ShaderUniforms.cpp"
    "src/Engine/Shader/UniformBlock.h"
    "src/Engine/Shader/UniformBlock.cpp"
)

SET(CAMERA
    "src/Engine/Camera/Camera.h"
    "src/Engine/Camera/Camera.cpp"
)

SET(MESH
    "src/Engine/Mesh/VertexAttribute.h"
    "src/Engine/Mesh/VertexAttribute.cpp"
    "src/Engine/Mesh/Mesh.h"
    "src/Engine/Mesh/Mesh.cpp"
)

SET(ENGINE
    ${SHADER}
    ${CAMERA}
    ${MESH}
    "src/Engine/Engine.h"
)

SET(SOURCE
    ${UTILS}
    ${ENGINE}
    "src/main.cpp"
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE})

SET(LIBS GLAD STB_IMAGE glm::glm assimp::assimp glfw OpenGL::GL)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})

TARGET_LINK_OPTIONS(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(STATUS "Debug mode enabled")
    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC DEBUG)
ENDIF()

IF(CMAKE_EXPORT_COMPILE_COMMANDS)
    FILE(CREATE_LINK
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/build/compile_commands.json"
        SYMBOLIC
    )
    MESSAGE(STATUS "Compile Commands symbolic link created")
ENDIF()

