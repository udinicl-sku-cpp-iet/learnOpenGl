#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "shader.hpp"

#ifdef EX2
// ImGui Headers
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float triangleVertices[] = {
  // positions         // colors
  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
  -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
}; 



void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);



int main(void){   
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnOpenGL", NULL, NULL);
  assert(window!=NULL);
  glfwMakeContextCurrent(window);
  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#ifdef EX2
  //-------------IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
    
  float currentOffset_x = 0.0f;
  float currentOffset_y= 0.0f;
  //--------------------
#endif

  //--------------SHADER
#ifdef STANDARD
  Shader shader("./glsl_shader/shader.vs"    ,"./glsl_shader/shader.fs"    );
#endif
#ifdef EX1
  Shader shader("./glsl_shader/shader_ex1.vs","./glsl_shader/shader.fs"    );
#endif
#ifdef EX2
  Shader shader("./glsl_shader/shader_ex2.vs","./glsl_shader/shader.fs"    );
#endif
#ifdef EX3
  Shader shader("./glsl_shader/shader_ex3.vs","./glsl_shader/shader_ex3.fs");
#endif

 

  
  unsigned int VAO,VBO;
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers     (1, &VBO);
    
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  
  
  glBufferData(GL_ARRAY_BUFFER,                     sizeof(triangleVertices), triangleVertices ,   GL_STATIC_DRAW);  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);  

  ///UNBINDING


  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);



  
  //we are declaring a vertices buffer object

  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);uncomment for wireframe
  while(!glfwWindowShouldClose(window)){
#ifdef EX2
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Finestra e Slider
    ImGui::Begin("Offset Control");
    ImGui::SliderFloat("X Position", &currentOffset_x, -0.5f, 0.5f);
    ImGui::SliderFloat("Y Position", &currentOffset_y, -0.5f, 0.5f);
    ImGui::End();
#endif
        // -----------------------------
    // input
    processInput(window);
    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
    shader.use();
#ifdef EX2
    shader.setFloat("offset_x",currentOffset_x);
    shader.setFloat("offset_y",currentOffset_y);
#endif
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
#ifdef EX2
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
  
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
#ifdef EX2
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers     (1, &VBO);
  shader.destroy();
  glfwTerminate();
}

void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  (void)window;
  glViewport(0, 0, width, height);
}  
