#version 450 core

in vec3 vs_lightPos;

out vec4 out_color;

uniform sampler2D u_posTexture;
uniform sampler2D u_normalTexture;

uniform vec2 u_screenSize;

void main(){

    vec2 texels = gl_FragCoord.xy / u_screenSize;

    vec3 vs_normal = (texture(u_normalTexture, texels)).xyz;
    vec3 vs_pos = (texture(u_posTexture, texels)).xyz;

    float specularIntensity = 0.5;
    float diffuseIntensity = 0.5;
    float specularPow = 32.0;

    vec3 light_dir = normalize(vs_lightPos - vs_pos);
    vec3 reflection_dir = -light_dir + 2*dot(vs_normal, light_dir)*vs_normal; 
    vec3 view_dir = vec3(0.0, 0.0, 1.0);

    float diffuseContrib = diffuseIntensity * max(dot(vs_normal, light_dir), 0);
    float specularContrib = specularIntensity * pow(max(dot(view_dir, reflection_dir), 0), specularPow);
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 color = ambient + vec3(diffuseContrib) + vec3(specularContrib);  
    out_color = vec4(color, 1.0);
}