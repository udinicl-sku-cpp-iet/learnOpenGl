#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
#ifdef EX4
float mix_value = 0.5f;
#endif

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *glfwInitWindow(const char *name);

#if defined(STANDARD) || defined(EX1) || defined(EX4)
float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};
#endif
#ifdef EX2
float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   2.0f, 2.0f,   // top right
     0.5f, -0.5f, 0.0f,   2.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 2.0f    // top left 
};
#endif
#ifdef EX3
float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   0.55f, 0.55f,   // top right
     0.5f, -0.5f, 0.0f,   0.55f, 0.45f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.45f, 0.45f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.45f, 0.55f    // top left 
};
#endif
unsigned int rectangleIndices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};


int main(void){   
  GLFWwindow* window = glfwInitWindow("LearnOpenGL");
 

  //---LOAD TEXTURE----
  unsigned int texture[2];
  glGenTextures(2, texture);
  
  int width, height, nrChannels;
  unsigned char *data;
  
  data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
#ifdef EX2
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
#ifdef EX3
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  //----------------------------------------------------
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
#ifdef EX2
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif
#ifdef EX3
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  //----------------------------------------------------


  //---VAO,VBO INITIALIZATION
  unsigned int VAO,VBO,EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers           (1, &VBO);
  glGenBuffers           (1, &EBO);
  
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,         VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices,  GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vertices),         vertices,          GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  //------------------------------------

  //--SHADER
#if defined(STANDARD) || defined(EX2) || defined(EX3)
  Shader shader("./glsl_shader/shader.vs","./glsl_shader/shader.fs");
#endif
#ifdef EX1
  Shader shader("./glsl_shader/shader.vs","./glsl_shader/shader_ex1.fs");
#endif
#ifdef EX4
  Shader shader("./glsl_shader/shader.vs","./glsl_shader/shader_ex4.fs");
#endif
  shader.use();
  shader.setInt("texture1", 0); 
  shader.setInt("texture2", 1); 
  //------------------------------------
  
  while(!glfwWindowShouldClose(window)){
  
    processInput(window);
  
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
#ifdef EX4
    shader.setFloat("mix_value", mix_value); // or with shader class
#endif

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
  shader.destroy();
  glfwTerminate();
}


void        processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
#ifdef EX4
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    if (mix_value <= 1.0f)     mix_value += 0.005f;
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    if (mix_value >= 0.0f)     mix_value -= 0.005f;
  }
#endif
}
void        framebuffer_size_callback(GLFWwindow* window, int width, int height){
  (void)window;//warning
  glViewport(0, 0, width, height);
}  
GLFWwindow *glfwInitWindow(const char *name){
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,name, NULL, NULL);
  assert(window!=NULL);
  glfwMakeContextCurrent(window);
  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}
