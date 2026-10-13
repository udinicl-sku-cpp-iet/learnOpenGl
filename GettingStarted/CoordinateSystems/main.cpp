#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef EX1
// ImGui Headers
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *glfwInitWindow(const char *name);
#ifdef EX1
void ImGui_Init(GLFWwindow *window, const char *glsl_version);
void ImGui_Update(float *fov, float *aspect_ratio, float *view_x, float *view_y, float *view_z, float *rotation_x, float *rotation_y, float *rotation_z);
void ImGui_Render();
void ImGui_Cleanup();
#endif




float vertices[] = {
  //vertices              //texture coord
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};


int main(void){

  GLFWwindow* window = glfwInitWindow("LearnOpenGL");

#ifdef EX1
  ImGui_Init(window,"#version 330 core");

  float fov          = 45.0f;
  float aspect_ratio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);

  float view_x       =  0.0f;
  float view_y       =  0.0f;
  float view_z       = -3.0f;
  float x_rotation   =  0.0f;
  float y_rotation   =  0.0f;
  float z_rotation   =  0.0f;
#endif
  //---LOAD TEXTURE----
  unsigned int texture[2];
  glGenTextures(2, texture);
  
  int width, height, nrChannels;
  unsigned char *data;
  
  data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);


  //---VAO,VBO INITIALIZATION
  unsigned int VAO,VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers     (1, &VBO);

  
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,         VBO);

  glBufferData(GL_ARRAY_BUFFER,         sizeof(vertices),         vertices,          GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  //--SHADER
  Shader shader("./glsl_shader/shader.vs","./glsl_shader/shader.fs");
  shader.use();
  shader.setInt("texture1", 0); 
  shader.setInt("texture2", 1);
  
  while(!glfwWindowShouldClose(window)){
#ifdef EX1
    ImGui_Update(&fov, &aspect_ratio, &view_x, &view_y, &view_z,  &x_rotation, &y_rotation, &z_rotation);
#endif
  
    processInput(window);
  
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    //----------------------
    for(size_t i = 0; i < (sizeof(cubePositions)/sizeof(cubePositions[0])); i++){
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = glm::mat4(1.0f);
      glm::mat4 projection;
      
      model = glm::translate(model, cubePositions[i]);
#ifdef EX2
      if (i%3 == 0 )
#endif
      model = glm::rotate(model,  (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

#ifdef EX1
      view  =      glm::rotate(view, x_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
      view  =      glm::rotate(view, y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
      view  =      glm::rotate(view, z_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      view  =      glm::translate(view, glm::vec3(view_x, view_y, view_z));      
      projection = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
#else
      view  =      glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      projection = glm::perspective(glm::radians(45.0f),
				    (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT))
				    , 0.1f, 100.0f);
#endif          
      shader.setMat4("model",     model);
      shader.setMat4("view",      view);
      shader.setMat4("projection",projection);

      glDrawArrays(GL_TRIANGLES, 0, 36);     
    }
    //----------------------    
#ifdef EX1
    ImGui_Render();
#endif
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
#ifdef EX1
  ImGui_Cleanup();
#endif
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers     (1, &VBO);
  shader.destroy();
  glfwTerminate();
}


void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
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
  glEnable(GL_DEPTH_TEST);

  return window;
}
#ifdef EX1
void ImGui_Init(GLFWwindow *window, const char *glsl_version) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io; 
    
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

}
void ImGui_Update(float *fov, float *aspect_ratio, float *view_x, float *view_y, float *view_z, float *rotation_x, float *rotation_y, float *rotation_z) {
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // 1. Finestra e Slider
  ImGui::Begin("Settings");
  ImGui::SliderFloat("Fov",          fov         , 10.0f, 80.0f);
  ImGui::SliderFloat("Aspect Ratio", aspect_ratio,
		     (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT))-2.0f,
		     (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT))+2.0f);

  ImGui::SliderFloat("View pos_x"  , view_x, -10.0f, 10.0f);
  ImGui::SliderFloat("View pos_y"  , view_y, -10.0f, 10.0f);
  ImGui::SliderFloat("View pos_z"  , view_z, -10.0f, 10.0f);
  ImGui::SliderFloat("View rotation_x"  , rotation_x, -10.0f, 10.0f);
  ImGui::SliderFloat("View rotation_y"  , rotation_y, -10.0f, 10.0f);
  ImGui::SliderFloat("View rotation_z"  , rotation_z, -10.0f, 10.0f);
  ImGui::End();
}
void ImGui_Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGui_Cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
#endif
