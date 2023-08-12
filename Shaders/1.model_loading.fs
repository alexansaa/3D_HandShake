#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}

// #version 330 core
// out vec4 FragColor;

// in vec2 TexCoords;

// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_custom;

// void main()
// {
//     vec4 tmpColor = vec4(0.0);
//     vec4 texColor = texture(texture_diffuse1, TexCoords);
//     tmpColor += texColor;
//     vec4 texColor1 = texture(texture_custom, TexCoords);
//     tmpColor += texColor1;
//     FragColor = tmpColor;
// }