#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// output color drawn to display
out vec3 color;

// texture data
uniform sampler2D myTextureSampler;

void main(){
    color = texture( myTextureSampler, UV).rgb;
}