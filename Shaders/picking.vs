#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  projection * view * model * vec4(aPos, 1.0f);

}

//#version 330 core
//layout (location = 0) in vec3 aPos;
//flat out int ObjectID;
//uniform int objID;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//void main() {
//    gl_Position = projection * view * model * vec4(aPos, 1.0f);
//    ObjectID = objID;
//}