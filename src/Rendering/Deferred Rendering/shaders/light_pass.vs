#version 450 core

layout(location = 0) in vec4 pos;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);
uniform vec3 u_lightPos;

out vec3 vs_lightPos;

void main(){
    vs_lightPos = normalize((u_view * u_model * vec4(u_lightPos, 1.0)).xyz);
    gl_Position = u_projection * u_view * u_model * pos;
}