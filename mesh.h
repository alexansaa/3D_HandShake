#ifndef MESH_H
#define MESH_H

//#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader/shader.h"

#include <string>
#include <vector>

#include "inputAdd.h"
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct SimpleVertex {
    // position
    glm::vec3 Position;
    // texCoords
    //glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<SimpleVertex> simpleVertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    aiColor3D    color;
    unsigned int VAO;

    // constructor
    Mesh(vector<SimpleVertex> vertices, vector<unsigned int> indices, aiColor3D color)
    {
        this->simpleVertices = vertices;
        this->indices = indices;
        this->color = color;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMeshCustom();
    }

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    // render the mesh into a texture, for 2d compatibility
    // la textura debe estar configurada y anclada antes de haber llamado a esta funcion
    // el frame buffer debe ser "pintado" (clear color bit) antes de llamar a esta funcion
    // y nunca ser borrado este buffer entre llamadas a esta funcion caso contrario se borra
    // lo que se encontraba dibujado previamente
    void DrawIntoTextureImport(Shader& shader)
    {      
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        unsigned int customNr = 1;

        //std::cout << "Number of textures: " + std::to_string(textures.size()) << std::endl;
        unsigned int i = 0;
        for (i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            //std::cout << "Texture type: " + textures[i].type << std::endl;
            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular") {
                number = std::to_string(specularNr++); // transfer unsigned int to string
            }
            else if (name == "texture_normal") {
                number = std::to_string(normalNr++); // transfer unsigned int to string
            }
            else if (name == "texture_height") {
                number = std::to_string(heightNr++); // transfer unsigned int to string
            }
            else if (name == "texture") {
                name = "texture_diffuse";
                number = std::to_string(customNr++);
            }

            //std::cout << "Texture name: " + name + " " + number << std::endl;
            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // preparar la renderizacion
        shader.use();

        // aplico color especifico de la mesh
        //float r_color = static_cast<float>(color.r);
        //float g_color = static_cast<float>(color.g);
        //float b_color = static_cast<float>(color.b);

        //std::cout << "Colors: " + std::to_string(r_color) + ", " + std::to_string(g_color) + ", " + std::to_string(b_color) << std::endl;

        //glm::vec4 myColor(r_color, g_color, b_color, 1.0f);

        //shader.setBool("useTexture", useTexure);
        //shader.setBool("useColor", useColor);
        //shader.setVec4("uColor", myColor);
        //shader.setFloat("textureMixFactor", textureMixRate);

        glBindVertexArray(VAO);
        glEnable(GL_DEPTH_TEST);        

        // elemento dibujado
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    // render the mesh into a texture, for 2d compatibility
    // la textura debe estar configurada y anclada antes de haber llamado a esta funcion
    // el frame buffer debe ser "pintado" (clear color bit) antes de llamar a esta funcion
    // y nunca ser borrado este buffer entre llamadas a esta funcion caso contrario se borra
    // lo que se encontraba dibujado previamente
    // Los modelos dibujados con este metodo no presentan texturas, solamente colores por cada triangulo
    void DrawIntoTextureCustom(Shader& shader) {
        // aplico color especifico de la mesh
        float r_color = static_cast<float>(color.r);
        float g_color = static_cast<float>(color.g);
        float b_color = static_cast<float>(color.b);

        //std::cout << "Colors: " + std::to_string(r_color) + ", " + std::to_string(g_color) + ", " + std::to_string(b_color) << std::endl;

        glm::vec4 myColor(r_color, g_color, b_color, 1.0f);

        shader.setVec4("uColor", myColor);

        glBindVertexArray(VAO);
        glEnable(GL_DEPTH_TEST);
        glEnableVertexAttribArray(0);

        // elemento dibujado
        glClear(GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        glm::vec4 myColorBlack(0.0f, 0.0f, 0.0f, 1.0f);
        shader.setVec4("uColor", myColorBlack);
        //glDisable(GL_CULL_FACE);
        glLineWidth(5.0f);
        glDrawElements(GL_LINE_LOOP, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glLineWidth(1.0f);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    // not working
    void DrawObjectsIdPixel(Shader& shader, aiColor3D idColor) {
        //setupMeshPicking();
        //// aplico color especifico de la mesh
        //float r_color = static_cast<float>(idColor.r);
        //float g_color = static_cast<float>(idColor.g);
        //float b_color = static_cast<float>(idColor.b);

        //glm::vec4 myColor(r_color, g_color, b_color, 1.0f);

        //shader.setVec4("PickingColor", myColor);

        //glBindVertexArray(VAO);
        //glEnableVertexAttribArray(0);
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);
        //glEnable(GL_CULL_FACE);

        //// elemento dibujado
        //glClearDepth(0.0);
        //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        ////glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0);
        //glDisableVertexAttribArray(0);

        //setupMeshCustom();
    }




private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }

    // initializes all the buffers object/arrays custom
    void setupMeshCustom()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, simpleVertices.size() * sizeof(SimpleVertex), &simpleVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
        // vertex texture coords
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, TexCoords));
        //glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    // not working
    void setupMeshPicking()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, simpleVertices.size() * sizeof(SimpleVertex), &simpleVertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }
};
#endif