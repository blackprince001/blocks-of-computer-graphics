#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 2) in vec2 tex_coords;

out vec2 image_tex_coords;

uniform mat4 transform;

void main() {
    gl_Position =  transform * vec4(a_pos, 1.0);
    
    image_tex_coords = vec2(tex_coords.x, tex_coords.y);
}
