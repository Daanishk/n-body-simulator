#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;
out vec3 vFragPos;   // world-space position
out vec3 vNormal;    // world-space normal

uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;

    // normal: transform object-space normal by model (ok for uniform scale)
    vNormal = normalize(mat3(model) * aPos);

    vColor = aColor;

    gl_Position = camMatrix * worldPos;
};