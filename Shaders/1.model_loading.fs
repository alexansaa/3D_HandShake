#version 330 core

//in vec3 vertexColor;
uniform vec3 uColor;
out vec4 FragColor;

void main() {
    //FragColor = vec4(vertexColor, 1.0);
    FragColor = vec4(uColor, 1.0f);
}

//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;

//void main()
//{    
//    FragColor = texture(texture_diffuse1, TexCoords);
//}