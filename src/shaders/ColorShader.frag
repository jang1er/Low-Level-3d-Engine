#version 330 core
// output color drawn to display
out vec3 color;
// Interpolated color values from vertex shader
in vec3 fragmentColor;

void main(){
    color = fragmentColor;
}