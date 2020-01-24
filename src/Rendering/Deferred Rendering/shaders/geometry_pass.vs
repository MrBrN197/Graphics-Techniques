
#version 450 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 normal;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

out vec3 vs_normal;
out vec3 vs_pos;

void main(){
    gl_Position = u_projection * u_view * u_model * pos;

    vs_normal = normalize(u_view * u_model * vec4(normal, 0.0)).xyz;
    vs_pos = (u_view * u_model * pos).xyz;
}