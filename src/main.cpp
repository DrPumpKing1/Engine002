#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>

#include "Engine.h"

const unsigned int WIDTH{800};
const unsigned int HEIGHT{600};
unsigned int currentWidth{WIDTH};
unsigned int currentHeight{HEIGHT};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

float deltaTime{0.0f};
float lastTime{0.0f};

float lastCursorX{WIDTH / 2.0f};
float lastCursorY{HEIGHT / 2.0f};
bool firstCursorClick{true};
bool cursorInGame{false};

static const float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
};

static const unsigned int cubeIndices[] = {
    // back  (z = -0.5)  -> normal -Z  (invertido)
    0,  2,  1,   2,  0,  3,
    // front (z = +0.5)  -> normal +Z  (ya estaba OK)
    4,  5,  6,   6,  7,  4,
    // left  (x = -0.5)  -> normal -X  (ya estaba OK)
    8,  9,  10,  10, 11, 8,
    // right (x = +0.5)  -> normal +X  (invertido)
    12, 14, 13,  14, 12, 15,
    // bottom(y = -0.5)  -> normal -Y  (ya estaba OK)
    16, 17, 18,  18, 19, 16,
    // top   (y = +0.5)  -> normal +Y  (invertido)
    20, 22, 21,  22, 20, 23
};

int main(void) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(currentWidth, currentHeight,
                                        "Hello World", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  {
      GLuint VAO, VBO, EBO;
      glCreateVertexArrays(1, &VAO);
      glCreateBuffers(1, &VBO);
      glCreateBuffers(1, &EBO);

      glNamedBufferStorage(VBO, sizeof(cubeVertices), cubeVertices, 0);
      glNamedBufferStorage(EBO, sizeof(cubeIndices), cubeIndices, 0);

      glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
      glVertexArrayElementBuffer(VAO, EBO);

      glEnableVertexArrayAttrib(VAO, 0);
      glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
      glVertexArrayAttribBinding(VAO, 0, 0);

      glEnableVertexArrayAttrib(VAO, 1);
      glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
      glVertexArrayAttribBinding(VAO, 1, 0);

      std::filesystem::path shadersPath = std::filesystem::current_path() / "resources" / "shaders";
      std::filesystem::path vertexShaderPath = shadersPath / "test.vs";
      std::filesystem::path fragmentShaderPath = shadersPath / "test.fs";
      Shader vertex(vertexShaderPath.string(), ShaderType::VERTEX);
      Shader fragment(fragmentShaderPath.string(), ShaderType::FRAGMENT);
      ShaderProgram program;
      program.AttachShader(std::move(vertex));
      program.AttachShader(std::move(fragment));
      program.Compile();

      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CCW);

      glEnable(GL_DEPTH_TEST);

      glClearColor(0.8f, 0.6f, 1.0f, 1.0f);
      while (!glfwWindowShouldClose(window)) {
          float currentTime = static_cast<float>(glfwGetTime());
          deltaTime = currentTime - lastTime;
          float fps = 1.0f / deltaTime;
          lastTime = currentTime;

          glfwSetWindowTitle(
                  window, ("Basic OpenGL Engine - FPS: " + std::to_string(fps)).c_str());

          glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
          glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(currentWidth) / currentHeight, 0.1f, 100.0f);
          glm::mat4 model(1.0f);
          model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
          model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
          model = glm::rotate(model, glm::radians(45.0f * currentTime), glm::vec3(1.0, 1.0, 0.0));

          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          program.Bind();
          program.SetMat4("model", model);
          program.SetMat4("view", view);
          program.SetMat4("projection", projection);

          glBindVertexArray(VAO);
          glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
          glBindVertexArray(0);

          glfwSwapBuffers(window);

          glfwPollEvents();
      }
  }

  glfwTerminate();
  return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  currentWidth = width;
  currentHeight = height;
  glViewport(0, 0, width, height);
}
