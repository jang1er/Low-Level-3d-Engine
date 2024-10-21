#version 330 core
// vertex location data
layout(location = 0) in vec3 vertexPosition_modelspace;
// vertex color data
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

// Model View Projection Matrix
uniform mat4 MVP;

void main(){
    // final position for the vertex: MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    fragmentColor = vertexColor;
}