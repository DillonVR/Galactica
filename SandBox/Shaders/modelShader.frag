#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform float objectcolor;

void main()
{    
    FragColor = vec4 (objectcolor,0.67,0.23,1.0); //texture(texture_diffuse1, TexCoords);
   
}