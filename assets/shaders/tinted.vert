#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 3) in vec3 normal;//as tex_xoord takes => 2

out Varyings {
    vec4 color;
    vec3 normal;
    vec3 view;
    vec3 world;
    vec2 tex_coord;
} vs_out;

uniform mat4 transform;
uniform mat4 transform_IT; //(M^-1)T for normal
uniform mat4 VP;
uniform vec3 eye;

void main(){
    vec3 world = (transform * vec4(position, 1.0)).xyz;
    gl_Position = VP * vec4(world, 1.0);
    vs_out.normal = (transform_IT * vec4(normal, 0.0)).xyz;
    vs_out.view = eye - world;
    vs_out.world = world;
    vs_out.color = color;
}