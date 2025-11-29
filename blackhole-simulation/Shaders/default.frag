#version 330 core

out vec4 FragColor;


in vec3 color;
// in vec2 texCoord;

// Gets the Texture Unit from the main function
// uniform sampler2D tex0;


void main()
{
	FragColor = vec4(color, 1.0);
}