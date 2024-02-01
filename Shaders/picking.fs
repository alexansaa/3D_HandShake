#version 330 core

// Ouput data
out vec4 FragColor;

// Values that stay constant for the whole mesh.
uniform vec4 PickingColor;

void main(){

    FragColor = PickingColor;

}

// #version 330 core
// flat in int ObjectID;
// out vec4 FragColor;

// void main() {
//     FragColor = vec4(float(ObjectID) / 255.0, 0.0, 0.0, 1.0); // Color based on ObjectID
// }