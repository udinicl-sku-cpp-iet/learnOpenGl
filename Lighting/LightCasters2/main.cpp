#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
////------------------------
#include "shader.hpp"
#include "camera.hpp"
////------------------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
////------------------------
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
////------------------------
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool cameraActive = true; // La telecamera è attiva all'avvio
bool tabPressed = false;  // Usato per il toggle (click singolo)



// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
////------------------------


////------------------------
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
////------------------------
GLFWwindow *glfwInitWindow(const char *name);
unsigned int loadTexture(char const * path);

typedef struct ImGuiConfig{
  glm::vec3 lightPos;
  glm::vec4 backgroundColor;
  glm::vec3 scale;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
}ImGuiConfig;
void ImGui_Init(GLFWwindow *window, const char *glsl_version);
void ImGui_Update(ImGuiConfig *config);
void ImGui_Render();
void ImGui_Cleanup();
////------------------------


////------------------------
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

////------------------------
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;//for mouse
bool firstMouse = true;
////------------------------
Camera camera(glm::vec3(0,0,3));
////------------------------

////------------------------

int main(void){
////------------------------
  GLFWwindow* window = glfwInitWindow("LearnOpenGL");
  ImGui_Init(window,"#version 330 core");
  ImGuiConfig *config = (ImGuiConfig*)malloc(sizeof(ImGuiConfig));
  config->backgroundColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f); 
  config->lightPos =        glm::vec3(1.2f, 1.0f, 2.0f);
  config->scale =           glm::vec3(1.0f, 1.0f, 1.0f);
  config->ambient  =           glm::vec3(0.2f, 0.2f, 0.2f);
  config->diffuse  =           glm::vec3(0.5f, 0.5f, 0.5f);
  config->specular =           glm::vec3(1.0f, 1.0f, 1.0f);
  
////------------------------



  
  //---VAO,VBO INITIALIZATION
  unsigned int VAO,lightVAO,VBO;
  
  glGenVertexArrays(1, &VAO     );
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers     (1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER,         VBO);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vertices),         vertices,          GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glBindVertexArray(lightVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  

  //--SHADER
  Shader shader            ("./glsl_shader/shader.vs",            "./glsl_shader/shader.fs");
  Shader lightingCubeShader("./glsl_shader/light_cube_shader.vs","./glsl_shader/light_cube_shader.fs");

  //---LOAD TEXTURE----
  unsigned int texture[2];
  glGenTextures(2, texture);
  
  int width, height, nrChannels;
  unsigned char *data;
  
  data = stbi_load("./textures/container2.png", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  data = stbi_load("./textures/container2_specular.png", &width, &height, &nrChannels, 0);
  assert(data);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  
  loadTexture("./textures/container2.png");
  loadTexture("./textures/container2_specular.png");
  
  
  shader.use();
  shader.setInt("material.diffuse", 0);
  shader.setInt("material.specular",1);
 


  
  while(!glfwWindowShouldClose(window)){
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    ////------------------------    
    ImGui_Update(config);
    ////------------------------
    
    processInput(window);
  
    glClearColor(config->backgroundColor.x,config->backgroundColor.y,config->backgroundColor.z,config->backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.setVec3("light.position", config->lightPos);
    shader.setVec3("viewPos"   ,camera.Position);

    
    shader.setVec3("light.ambient",config->ambient); 
    shader.setVec3("light.diffuse",config->diffuse);
    shader.setVec3("light.specular", config->specular);


    shader.setFloat("material.shininess", 64.0f);
    
    shader.setFloat("light.constant",  1.0f);
    shader.setFloat("light.linear",    0.09f);
    shader.setFloat("light.quadratic", 0.032f);	

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)), 0.1f, 100.0f);
    shader.setMat4("view",      view);
    shader.setMat4("projection",projection);

    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model",     model);




    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++){
      // calculate the model matrix for each object and pass it to shader before drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    model = glm::translate(model, config->lightPos);
    model = glm::scale(model,     config->scale);
    
    lightingCubeShader.use();
    lightingCubeShader.setMat4("view",      view);
    lightingCubeShader.setMat4("projection",projection);
    lightingCubeShader.setMat4("model",     model);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    ////------------------------
    ImGui_Render();
    ////------------------------


    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }

  ////------------------------
  ImGui_Cleanup();
  ////------------------------

  
  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers     (1, &VBO);
  ////------------------------
  shader.destroy();
  lightingCubeShader.destroy();
  ////------------------------
  glfwTerminate();
}

////------------------------
void processInput(GLFWwindow *window){

  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed)
    {
      cameraActive = !cameraActive;
      if (cameraActive) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      } else {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      firstMouse = true; // Previene scatti della telecamera
      tabPressed = true;
    }
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    tabPressed = false;

  
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::FORWARD,deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::BACKWARD,deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::LEFT,deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT,deltaTime);
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::UP,deltaTime);
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::DOWN,deltaTime);
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
    
  }
}



////------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  (void)window;//warning
  glViewport(0, 0, width, height);
}  

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

  if(!cameraActive) return

  (void)window;//warning
  if (firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

  if(!cameraActive) return

  (void)window;
  (void)xoffset;
  camera.ProcessMouseScroll(yoffset);
}
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glEnable(GL_DEPTH_TEST);

  return window;
}
void ImGui_Init(GLFWwindow *window, const char *glsl_version) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io; 
    
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

}
void ImGui_Update(ImGuiConfig *config) {
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Background Color", glm::value_ptr(config->backgroundColor));
  ImGui::ColorEdit3("ambient Color", glm::value_ptr(config->ambient));
  ImGui::ColorEdit3("diffuse Color", glm::value_ptr(config->diffuse));
  ImGui::ColorEdit3("specular Color", glm::value_ptr(config->specular));
  ImGui::SliderFloat("Light Pos x "  , &config->lightPos.x, -10.0f, 10.0f);
  ImGui::SliderFloat("Light Pos y"   , &config->lightPos.y, -10.0f, 10.0f);
  ImGui::SliderFloat("Light Pos z"   , &config->lightPos.z, -10.0f, 10.0f);
  ImGui::SliderFloat("Scale x "  , &config->scale.x, -10.0f, 10.0f);
  ImGui::SliderFloat("Scale y"   , &config->scale.y, -10.0f, 10.0f);
  ImGui::SliderFloat("Scale z"   , &config->scale.z, -10.0f, 10.0f);
  


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

