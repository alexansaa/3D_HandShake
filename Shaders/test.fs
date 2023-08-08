#version 330 core

// Input from vertex shader
in vec2 TexCoord;

// Texturing
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureMixFactor; // Interpolation value for textures

// Color
uniform vec4 uColor;

// Switch between texturing and coloring
uniform bool useTexture;
uniform bool useColor;

out vec4 FragColor;

void main()
{
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureMixFactor);

    if (useTexture && useColor) {
        FragColor = texColor * uColor;
    } else if (useTexture) {
        FragColor = texColor;
    } else if (useColor) {
        FragColor = uColor;
    } else {
        // No texturing or coloring, use a default color or behavior
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
