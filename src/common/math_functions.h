#pragma once

#define PI 3.141592653589793F

#include "core.h"

union vec4{
    struct {
        float x, y, z, w;
    };
    uint32 r, g, b, a;
};

union vec3{
    struct {
        float x, y, z;
    };
    uint32 r, g, b;
};

struct mat4{
    union{
        float elements[4*4] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        vec4 columns[4];
    };
};

mat4 operator*(const mat4& left, const mat4& right){

    mat4 result;
    float sum;

    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            sum = 0;
            for(int e = 0; e < 4; e++){
                sum += left.elements[e * 4 + r] * right.elements[c * 4 + e];
            }
            result.elements[c * 4 + r] = sum;
        }
    }

    return result;

}

vec4 operator*(const mat4& left, const vec4& right){
    vec4 result; 

    result.x = left.columns[0].x * right.x + left.columns[1].x * right.y + left.columns[2].x * right.z + left.columns[3].x * right.w;
    result.y = left.columns[0].y * right.x + left.columns[1].y * right.y + left.columns[2].y * right.z + left.columns[3].y * right.w;
    result.z = left.columns[0].z * right.x + left.columns[1].z * right.y + left.columns[2].z * right.z + left.columns[3].z * right.w;
    result.w = left.columns[0].w * right.x + left.columns[1].w * right.y + left.columns[2].w * right.z + left.columns[3].w * right.w;

    return result;
}


vec3& operator+=(vec3& left, const vec3& right){
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}
vec3& operator-=(vec3& left, const vec3& right){
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}
vec3& operator*=(vec3& left, const vec3& right){
    left.x *= right.x;
    left.y *= right.y;
    left.z *= right.z;
    return left;
}
vec3& operator/=(vec3& left, const vec3& right){
    left.x /= right.x;
    left.y /= right.y;
    left.z /= right.z;
    return left;
}
vec3& operator*=(vec3& a, float b){
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}
vec3& operator*=(float a, vec3& b){
    b.x *= a;
    b.y *= a;
    b.z *= a;
    return b;
}
vec3& operator/=(vec3& a, float b){
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}


vec3 operator+(vec3 left, const vec3& right){
    left += right;
    return left;
}
vec3 operator-(vec3 left, const vec3& right){
    left -= right;
    return left;
}
vec3 operator*(vec3 left, const vec3& right){
    left *= right;
    return left;
}
vec3 operator/(vec3 left, const vec3& right){
    left /= right;
    return left;
}

vec3 operator*(vec3 a, float b){
    a *= b;
    return a;
}
vec3 operator*(float a, vec3 b){
    b *= a;
    return b;
}
vec3 operator/(vec3 a, float b){
    a /= b;
    return a;
}


// VEC4
vec4& operator+=(vec4& left, const vec4& right){
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    left.w += right.w;
    return left;
}
vec4& operator-=(vec4& left, const vec4& right){
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    left.w -= right.w;
    return left;
}
vec4& operator*=(vec4& left, const vec4& right){
    left.x *= right.x;
    left.y *= right.y;
    left.z *= right.z;
    left.w *= right.w;
    return left;
}
vec4& operator/=(vec4& left, const vec4& right){
    left.x /= right.x;
    left.y /= right.y;
    left.z /= right.z;
    left.w /= right.w;
    return left;
}
vec4& operator*=(vec4& a, float b){
    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
    return a;
}
vec4& operator*=(float a, vec4& b){
    b.x *= a;
    b.y *= a;
    b.z *= a;
    b.w *= a;
    return b;
}
vec4& operator/=(vec4& a, float b){
    a.x /= b;
    a.y /= b;
    a.z /= b;
    a.w /= b;
    return a;
}


vec4 operator+(vec4 left, const vec4& right){
    left += right;
    return left;
}
vec4 operator-(vec4 left, const vec4& right){
    left -= right;
    return left;
}
vec4 operator*(vec4 left, const vec4& right){
    left *= right;
    return left;
}
vec4 operator/(vec4 left, const vec4& right){
    left /= right;
    return left;
}

vec4 operator*(vec4 a, float b){
    a *= b;
    return a;
}
vec4 operator*(float a, vec4 b){
    b *= a;
    return b;
}
vec4 operator/(vec4 a, float b){
    a /= b;
    return a;
}

///////////////////////////////////////////////


mat4 PerspectiveTransform(float n, float f, float theta, float aspectRatio){

    float rad = theta/180.0f * PI; 

    float C = -(f+n)/(f-n);
    float D = -2*f*n / (f-n);

    mat4 result;
    memset(result.elements, NULL, sizeof(float) * 16);

    result.elements[0 * 4 + 0] = 1.0f/(tanf(rad) * aspectRatio);
    result.elements[1 * 4 + 1] = 1.0f/tanf(rad);
    result.elements[2 * 4 + 2] = C;
    result.elements[2 * 4 + 3] = -1.0f;
    result.elements[3 * 4 + 2] = D;

    return result;
}

mat4 Translate(float x, float y, float z){

    mat4 result;
    result.elements[3 * 4 + 0] = x;
    result.elements[3 * 4 + 1] = y;
    result.elements[3 * 4 + 2] = z;

    return result;
}

mat4 RotateX(float angle){
    mat4 result;
    float rad = angle/180.0f * PI;
    result.elements[1 * 4 + 1] =  cos(rad); 
    result.elements[2 * 4 + 1] = -sin(rad);
    result.elements[1 * 4 + 2] =  sin(rad);
    result.elements[2 * 4 + 2] =  cos(rad);
    return result;
}
mat4 RotateY(float angle){
    mat4 result;
    float rad = angle/180.0f * PI;
    result.elements[0 * 4 + 0] =  cos(rad); 
    result.elements[2 * 4 + 0] =  sin(rad);
    result.elements[0 * 4 + 2] = -sin(rad);
    result.elements[2 * 4 + 2] =  cos(rad);
    return result;
}
mat4 RotateZ(float angle){
    mat4 result;
    float rad = angle/180.0f * PI;
    result.elements[0 * 4 + 0] =  cos(rad); 
    result.elements[1 * 4 + 0] = -sin(rad);
    result.elements[0 * 4 + 1] =  sin(rad);
    result.elements[1 * 4 + 1] =  cos(rad);
    return result;
}

