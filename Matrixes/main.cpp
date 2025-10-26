#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float rectangleVertices[] = {//ORIGINAL
  // positions          // colors           // texture coords
  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
float mix_value=0.2f;//bruttissimo

// float rectangleVertices[] = {//EXERCISE N1
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
// };
// float rectangleVertices[] = {//EXERCISE N2
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left 
// };
unsigned int rectangleIndices[] = {  // note that we start from 0!
  0, 1, 3,   // first triangle
  1, 2, 3    // second triangle
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
  //----------------------------------------

 

  
  unsigned int VAO,VBO,EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers           (1, &VBO);
  glGenBuffers           (1, &EBO);
    
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,                      VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices,  GL_STATIC_DRAW);  
  glBufferData(GL_ARRAY_BUFFER,                     sizeof(rectangleVertices), rectangleVertices,GL_STATIC_DRAW);  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);  

  ///UNBINDING


  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,                      0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 

  shader.use(); // don't forget to activate the shader before setting uniforms!  
  glUniform1i(glGetUniformLocation(shader.ID, "ourTexture1"), 0); // set it manually
  shader.setInt("ourTexture1", 0); // or with shader class
  shader.setInt("ourTexture2", 8); // or with shader class

  

  float time;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//uncomment for wireframe
   while(!glfwWindowShouldClose(window)){
     // input
     time=(float)glfwGetTime();
     processInput(window);
     shader.use();
     shader.setFloat("mix_value",mix_value);
     // rendering commands here
     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT);


     glBindVertexArray(VAO);    

     glm::mat4 trans = glm::mat4(1.0f);
     trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
     trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));


     unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
     glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

     trans = glm::mat4(1.0f);
     trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
     trans = glm::scale(trans, glm::vec3(sin(time),sin(time), sin(time)));
     glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    



    
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers     (1, &VBO);
  glDeleteBuffers     (1, &EBO);
  //shaderNotDeleted!!
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
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}  
