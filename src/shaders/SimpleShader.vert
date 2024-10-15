#version 330 core
// vertex input data
layout(location = 0) in vec3 vertexPosition_modelspace;

// Model View Projection Matrix
uniform mat4 MVP;

void main(){
    // final position for the vertex: MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
}