#version 450 core

in vec3 vs_normal;
in vec3 vs_pos;

layout (location = 0) out vec3 out_pos;
layout (location = 1) out vec3 out_normal;

void main(){
    out_pos = vs_pos;
    out_normal = vs_normal;
}