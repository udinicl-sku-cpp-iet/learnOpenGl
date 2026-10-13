#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
  

class Shader
{
public:
  // the program ID
  unsigned int ID;
  
  // constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);
  // use/activate the shader
  void use();
  //delte shader
  void destroy();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;  
  void setInt(const std::string &name, int value) const;   
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, const glm::mat4& value) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
private:
  void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions  (std::ifstream::failbit | std::ifstream::badbit);
  try { 
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();		
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();		
  }
  catch(std::ifstream::failure& e){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  unsigned int vertex, fragment;
   
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::use() { 
  glUseProgram(ID);
}
void Shader::destroy() {
  glDeleteProgram(ID);
}  
void Shader::setBool(const std::string &name, bool value) const{         
  GLint location = glGetUniformLocation(ID, name.c_str());

  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not founded or unused in the shader ID " << ID << std::endl;
  }
  glUniform1i(location, value); 
}
void Shader::setInt(const std::string &name, int value) const{
  GLint location = glGetUniformLocation(ID, name.c_str());
  
  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not founded or unused in the shader ID " << ID << std::endl;
  }
  glUniform1i(location, value); 
}
void Shader::setFloat(const std::string &name, float value) const{
  GLint location = glGetUniformLocation(ID, name.c_str());

  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not founded or unused in the shader ID " << ID << std::endl;
  }
  glUniform1f(location, value); 
}
void Shader::setMat4(const std::string &name, const glm::mat4 &value) const{
  GLint location = glGetUniformLocation(ID, name.c_str());
  
  if (location == -1) {
    std::cerr << "WARNING: Uniform '" << name << "' not founded or unused in the shader ID " << ID << std::endl;
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    
    if (location == -1) {
        std::cerr << "WARNING: Uniform '" << name << "' not founded or unused in the shader ID " << ID << std::endl;
    }    
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type){
  int success;
  char infoLog[1024];
  if (type != "PROGRAM"){
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
		<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else{
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success){
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
		<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}
