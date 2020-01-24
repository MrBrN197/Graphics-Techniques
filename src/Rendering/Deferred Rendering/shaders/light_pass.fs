#version 450 core

in vec3 vs_lightPos;

out vec4 out_color;

uniform sampler2D u_posTexture;
uniform sampler2D u_normalTexture;

uniform vec2 u_screenSize;


float clamp01(float v){
    return max(0, min(v, 1));
}

void main(){

    vec2 texels = gl_FragCoord.xy / u_screenSize;

    vec3 vs_normal = (texture(u_normalTexture, texels)).xyz;
    vec3 vs_pos = (texture(u_posTexture, texels)).xyz;

    float specularIntensity = 0.5;
    float diffuseIntensity = 0.5;
    float specularPow = 5.0;

    vec3 lightDir = normalize(vs_lightPos - vs_pos);
    vec3 reflectionDir = -lightDir + 2*dot(vs_normal, lightDir)*vs_normal; 
    vec3 view_dir = vec3(0.0, 0.0, 1.0);

    float diffuseContrib = diffuseIntensity * clamp01(dot(vs_normal, lightDir));
    float specularContrib = specularIntensity * pow(clamp01(dot(view_dir, reflectionDir)), specularPow);
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 color = ambient + vec3(diffuseContrib) + vec3(specularContrib);  
    out_color = vec4(color, 1.0);
}