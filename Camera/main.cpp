#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef DEBUG
// ImGui Headers
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



float mix_value=0.2f;//bruttissimo
float view_fov=45.0f;//bruttissimo
float aspect_ratio= (float)SCR_WIDTH / (float)SCR_HEIGHT;//bruttissimo
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);//bruttissimo
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//bruttissimo
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);//bruttissimo

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float cubeVertices[] = {
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

  
#ifdef DEBUG
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
#endif
  

  //--------------SHADER
  Shader shader("./shader.vs","shader.fs");

  
  //--------------------LOAD TEXTURE
  stbi_set_flip_vertically_on_load(true);  
  glActiveTexture(GL_TEXTURE0);//non necessario(default)
  int width, height, nrChannels;
  unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
  unsigned int textureContainer;
  glGenTextures(1, &textureContainer);
  glBindTexture(GL_TEXTURE_2D, textureContainer);
  if(data){
    //std::cout<<nrChannels<<std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else{
    std::cerr<<"FAILED IN LOADING TEXTURE"<<std::endl;
  }
  assert(data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(GL_TEXTURE8);
  data=stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);
  unsigned int textureSmile;
  glGenTextures(1, &textureSmile);
  glBindTexture(GL_TEXTURE_2D, textureSmile);
  if(data){
    //std::cout<<nrChannels<<std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else{
    std::cerr<<"FAILED IN LOADING TEXTURE"<<std::endl;
  }
  assert(data);
  stbi_image_free(data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


 

  
  unsigned int VAO,VBO;


  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

    
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
 

  

  glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices), cubeVertices,GL_STATIC_DRAW);  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  

  ///UNBINDING


  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 

  shader.use(); // don't forget to activate the shader before setting uniforms!  
  shader.setInt("ourTexture1", 0); // or with shader class
  shader.setInt("ourTexture2", 8); // or with shader class


  
  
  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//uncomment for wireframe
  glEnable(GL_DEPTH_TEST);  
  while(!glfwWindowShouldClose(window)){

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    // input
    processInput(window);
    // rendering commands here
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

#ifdef DEBUG
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); 
#endif

   
    //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.2f, 0.2f, 0.2f));


    //--------------------CAMERA
    
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(view_fov), aspect_ratio, 0.1f, 100.0f);
    

   
    
    shader.use();
    shader.setFloat("mix_value",mix_value);        
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    
    glBindVertexArray(VAO);
  
    for(unsigned int i = 0; i < 10; i++){
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i; 
      if(i%3 ){	      
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));	
      }
      else{
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));	
      }
      shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

#ifdef DEBUG
    // Rendering
    // (Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // (Your code calls glfwSwapBuffers() etc.)
#endif
  
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers     (1, &VBO);
  glDeleteTextures(1, &textureContainer);
  glDeleteTextures(1, &textureSmile);

  if (shader.ID != 0) { // Controlla se l'ID è valido prima di cancellare
      glDeleteProgram(shader.ID);
  }
#ifdef DEBUG
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif

  glfwTerminate();
}















void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    if (mix_value <= 1.0f)     mix_value+=0.01f;
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    if (mix_value >= 0.0f)     mix_value -= 0.01f;
  }

  
  float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  // if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
  //   if (view_fov <= 180.0f)    
  //     view_fov += 0.5f;
  // }

  // if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
  //   if (view_fov >= 10.0f)    
  //     view_fov -= 0.5f;
  // }
  // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  //   if (aspect_ratio <= ((float)SCR_WIDTH / (float)SCR_HEIGHT)+1.0f)    
  //     aspect_ratio += 0.05f;
  // }

  // if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
  //   if (aspect_ratio >=  ((float)SCR_WIDTH / (float)SCR_HEIGHT)-1.0f)    
  //     aspect_ratio -= 0.05f;
  // }

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  (void)window; //for warning
  glViewport(0, 0, width, height);
}  
