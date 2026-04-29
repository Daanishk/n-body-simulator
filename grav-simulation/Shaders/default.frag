#version 330 core

out vec4 FragColor;


in vec3 vColor;
in vec3 vFragPos;
in vec3 vNormal;

uniform vec3 lightPos;    // sun position (world space)
uniform vec3 lightColor;  // sun color/intensity

uniform bool isEmissive;  


void main()
{
    if (isEmissive) {
        FragColor = vec4(vColor, 1.0);
        return;
    }
    vec3 ambient = 0.1 * lightColor;

    // diffuse term (Lambert)
    vec3 norm     = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * lightColor;

    vec3 lighting = ambient + diffuse;

    vec3 result = lighting * vColor;
    FragColor = vec4(result, 1.0);
}