#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float triangleVertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
}; 
float triangleVertices1[] = {//right
  0.0f, -0.5f, 0.0f,//left
  1.0f, -0.5f, 0.0f,//right
  0.5f,  0.5f, 0.0f//top
};
float triangleVertices2[] = {//left
  -1.0f, -0.5f, 0.0f,//left
  0.0f, -0.5f, 0.0f,//right
  -0.5f,  0.5f, 0.0f//top
};
float twoTriangleVertices[] = {
  //right
    0.0f, -0.5f, 0.0f,//left
    1.0f, -0.5f, 0.0f,//right
    0.5f,  0.5f, 0.0f,//top
    //left
   -1.0f, -0.5f, 0.0f,//left
    0.0f, -0.5f, 0.0f,//right
    -0.5f,  0.5f, 0.0f//top
};
// float rectangleVertices[] = {
//     // first triangle
//      0.5f,  0.5f, 0.0f,  // top right
//      0.5f, -0.5f, 0.0f,  // bottom right
//     -0.5f,  0.5f, 0.0f,  // top left 
//     // second triangle
//      0.5f, -0.5f, 0.0f,  // bottom right
//     -0.5f, -0.5f, 0.0f,  // bottom left
//     -0.5f,  0.5f, 0.0f   // top left
// };
float rectangleVertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int rectangleIndices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

const char *vertexShaderSource   = "#version 330 core                                                 \n"
                                                          "layout (location = 0) in vec3 aPos;                       \n"
                                                          // "out vec4 aColor;                                                   \n"
                                                          "void main(){                                                         \n"
                                                          "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                                          // "   aColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);                   \n"
                                                          "}                                                                           \0";
const char *fragmentShaderSource1 = "#version 330 core                                             \n"
                                                             "out vec4 FragColor;                                           \n"
                                                             // "in vec4 aColor;                                                   \n"
                                                             "uniform vec4 aColor;                                                   \n"
                                                             "void main(){                                                       \n"
                                                             "  FragColor = aColor;                                        \n"
                                                             "}                                                                        \0";
const char *fragmentShaderSource2 = "#version 330 core                                               \n"
                                                            "out vec4 FragColor;                                           \n"
                                                            "void main(){                                                       \n"
                                                            "  FragColor = vec4(1.0f, 0.8f, 0.0f, 1.0f);            \n"
                                                            "}                                                                        \0";
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
  int  success;
  char infoLog[512];
  //vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  //check glsl compile
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  //fragment shader
  unsigned int fragmentShader1;
  fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);
  //check glsl compile
  glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
   //fragment shader 2
  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);
  //check glsl compile
  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  //create shaderProgram
  unsigned int shaderProgram[2];
  shaderProgram[0] = glCreateProgram();
  glAttachShader(shaderProgram[0], vertexShader);
  glAttachShader(shaderProgram[0], fragmentShader1);
  glLinkProgram(shaderProgram[0]);
  //check glsl linking
  glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  shaderProgram[1] = glCreateProgram();
  glAttachShader(shaderProgram[1], vertexShader);
  glAttachShader(shaderProgram[1], fragmentShader2);
  glLinkProgram(shaderProgram[1]);
  //check glsl linking
  glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  
  unsigned int VAO[2],VBO[2];
  
  glGenVertexArrays(1, &VAO[0]);
  glGenBuffers(1, &VBO[0]);
  glGenVertexArrays(1, &VAO[1]);
  glGenBuffers(1, &VBO[1]);
  
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER,                    VBO[0]);
  
  
  //glBufferData(GL_ARRAY_BUFFER,                     sizeof(rectangleVertices), rectangleVertices ,   GL_STATIC_DRAW);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices),   rectangleIndices  ,   GL_STATIC_DRAW);
  
  glBufferData(GL_ARRAY_BUFFER,                     sizeof(triangleVertices1), triangleVertices1 ,   GL_STATIC_DRAW);  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  
  glEnableVertexAttribArray(0);  

  ///UNBINDING
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBindVertexArray(VAO[1]);
  glBufferData(GL_ARRAY_BUFFER,                     sizeof(triangleVertices2), triangleVertices2,   GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);



  
  //we are declaring a vertices buffer object

  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);uncomment for wireframe
  while(!glfwWindowShouldClose(window)){
     // input
    processInput(window);
    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
    glUseProgram(shaderProgram[0]);
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram[0], "aColor");
    assert(vertexColorLocation!=-1);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    // GLfloat dati[]={0.0f, greenValue, 0.0f, 1.0f};
    // glUniform4fv(vertexColorLocation, 1,dati);
    
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgram[1]);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawArrays(GL_TRIANGLES, 3, 6);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    // check and call events and swap the buffers
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  glDeleteProgram(shaderProgram[0]);
  glDeleteProgram(shaderProgram[1]);
  glfwTerminate();
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
