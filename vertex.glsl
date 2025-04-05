#version 330 core

// Define one vertex attribute (input): a vec3 at location 0 called aPos
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main() {
    // gl_Position is the output of the vertex shader, always vec4
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    
    color = aColor;
}