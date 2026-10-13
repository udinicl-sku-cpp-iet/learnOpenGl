#version 330 core


uniform vec3  objectColor;
in vec3 Result;
out vec4 FragColor;

void main(){

  FragColor = vec4(Result * objectColor, 1.0);

}
