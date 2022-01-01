#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 4) in vec3 normal;


out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

uniform mat4 transform;
uniform mat4 transform_IT;
uniform mat4 VP;
uniform vec3 eye;

void main(){

    vec3 world = (transform * vec4(position, 1.0)).xyz;
    gl_Position = VP * vec4(world, 1.0);
    vs_out.normal = (transform_IT * vec4(normal, 0.0)).xyz;
    vs_out.view = eye - world;
    vs_out.world = world;

    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
}