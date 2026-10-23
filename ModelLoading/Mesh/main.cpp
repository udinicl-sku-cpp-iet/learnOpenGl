#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
////------------------------
#include "shader.hpp"
#include "camera.hpp"

#include "model.hpp"
////------------------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
////------------------------
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
////------------------------
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

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
unsigned int loadTexture(char const * path);
////------------------------
GLFWwindow *glfwInitWindow(const char *name);
typedef struct ImGuiConfig{
  glm::vec4 backgroundColor;

}ImGuiConfig;
void ImGui_Init(GLFWwindow *window, const char *glsl_version);
void ImGui_Update(ImGuiConfig *config);
void ImGui_Render();
void ImGui_Cleanup();
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

  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);

  //--SHADER
  Shader shader            ("./glsl_shader/model.vs",            "./glsl_shader/model.fs");
  Model  ourModel             (("./models/backpack.obj"));


  
  shader.use();


  
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

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)), 0.1f, 100.0f);
    shader.setMat4("view",      view);
    shader.setMat4("projection",projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    shader.setMat4("model",     model);
    ourModel.Draw(shader);


    ////------------------------
    ImGui_Render();
    ////------------------------


    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }

  ////------------------------
  ImGui_Cleanup();
  ////------------------------

  
 
  shader.destroy();
 
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

