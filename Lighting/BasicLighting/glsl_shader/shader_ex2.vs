#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out vec3 Result;

uniform vec3  lightColor;
uniform vec3 lightPos;
uniform vec3  viewPos;
uniform int   shininess;
uniform float specularStrength;
uniform float ambientStrength ;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main(){                                             
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  
  vec3 Position = vec3(view * model * vec4(aPos, 1.0));
  vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
  vec3 LightPos = vec3(view * vec4(lightPos, 1.0));
  
  vec3 ambient = ambientStrength * lightColor;
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(LightPos - Position);  
    
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  vec3 viewDir = normalize( viewPos - Position);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * lightColor;
  
  vec3 result = (ambient + diffuse + specular);
  Result = result;
}

