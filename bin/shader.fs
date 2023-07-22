#version 330 core

out vec4 FragColor;

in vec2 texCoords;
in float lightStrength;

uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, texCoords);
    FragColor = vec4(color.r * lightStrength, color.g * lightStrength, color.b * lightStrength, 1);
    //FragColor = color;
}