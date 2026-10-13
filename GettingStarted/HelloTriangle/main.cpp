#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



#ifdef STANDARD
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  
#endif
#ifdef EX1
float vertices[] = {
  0.50f,  0.50f, 0.00f,     //first right triangle
  1.00f, -0.50f, 0.00f,  
  0.00f, -0.50f, 0.00f,  

 -0.50f, 0.50f, 0.00f,
 -1.00f,-0.50f, 0.00f,
  0.00f,-0.50f, 0.00f
};
#endif
#if defined(EX2) || defined(EX3)
float vertices1[] = {
  0.50f,  0.50f, 0.00f,     //first right triangle
  1.00f, -0.50f, 0.00f,  
  0.00f, -0.50f, 0.00f,  
};
float vertices2[] = {
 -0.50f, 0.50f, 0.00f,
 -1.00f,-0.50f, 0.00f,
  0.00f,-0.50f, 0.00f
};
#endif


const char *vertexShaderSource   = "#version 330 core                                                         \n"
                                   "layout (location = 0) in vec3 aPos;                                       \n"
                                   "void main(){                                                              \n"
                                   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);                       \n"
                                   "}                                                                         \0";

const char *fragmentShaderSource = "#version 330 core                                                         \n"
                                   "out vec4 FragColor;                                                       \n"
                                   "uniform vec4 aColor;                                                      \n"
                                   "void main(){                                                              \n"
                                   "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);                               \n"
                                   "}                                                                         \0";

#ifdef EX3
const char *fragmentShaderSource2= "#version 330 core                                                         \n"
                                   "out vec4 FragColor;                                                       \n"
                                   "uniform vec4 aColor;                                                      \n"
                                   "void main(){                                                              \n"
                                   "  FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);                               \n"
                                   "}                                                                         \0";
#endif


void        processInput(GLFWwindow *window);
void        framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *glfwInitWindow(const char *name);



int main(void){   
  GLFWwindow* window = glfwInitWindow("LearnOpenGL");

  //--------------SHADER
  int success;

  //vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  //check glsl compile
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  
  //fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  //check glsl compile
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char infoLog[512];    
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
#ifdef EX3
  //fragment shader
  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);
  //check glsl compile
  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
  if(!success){
    char infoLog[512];    
    glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  //create shaderProgram2
  unsigned int shaderProgram2;
  shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader  );
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);
  //check glsl linking
  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
  if(!success) {
    char infoLog[512];    
    glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  glDeleteShader(fragmentShader2);

#endif

  //create shaderProgram
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader  );
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  //check glsl linking
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    char infoLog[512];    
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return -1;
  }

  
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  //--------------------------------

#if defined(STANDARD) || defined(EX1)
  GLuint VBO;
  glGenBuffers(1,              &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);    

  glEnableVertexAttribArray(0);
#ifdef STANDARD
  GLuint EBO;
  glGenBuffers(1,                      &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
#endif

#if defined(EX2) || defined(EX3)
  GLuint VBO[2],VAO[2];
  glGenBuffers     (2, VBO);
  glGenVertexArrays(2, VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
  glBindVertexArray(VAO[0]);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
  glBindVertexArray(VAO[1]);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

#endif
 

  

  while(!glfwWindowShouldClose(window)){
    // input
    processInput(window);
    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

   

    //GLint desired_mode = (fmod(glfwGetTime(), 2.0) < 1.0) ? GL_FILL : GL_LINE;        
    GLint modes[2];
    glGetIntegerv(GL_POLYGON_MODE, modes);
    //if (modes[0] != desired_mode) {
    //glPolygonMode(GL_FRONT_AND_BACK, desired_mode);
    //}
    
    glUseProgram(shaderProgram);
#if defined(STANDARD) || defined(EX1)
    glBindVertexArray(VAO);
#ifdef STANDARD
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
#ifdef EX1
    glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
#endif
    

#if defined(EX2) || defined(EX3)
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
#ifdef EX3
    glUseProgram(shaderProgram2);
#endif
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
#endif
    
    glBindVertexArray(0);
    
    glfwSwapBuffers(window);    
    glfwPollEvents();
    
  }
#if defined(STANDARD) || defined(EX1)
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers     (1, &VBO);
#endif
#if defined(EX2) || defined(EX3)
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers     (2, VBO);
#ifdef EX3
  glDeleteProgram(shaderProgram2);
#endif
#endif
  glDeleteProgram(shaderProgram);

  glfwTerminate();
}



void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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

  return window;
}

