#ifndef MODEL_H
#define MODEL_H

//#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "shader/shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "mesh.h"

#include <stdio.h>

using namespace std;

// se define estatico para que se pueda definir en el main en vez de definir en el loop de renderizacion
static unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:   
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    //----------------------------------------------
    //Alex defnitions
    Model()
    {

    }
    //----------------------------------------------

    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool processCustom, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path, processCustom);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    aiColor3D myColor() {
        aiColor3D tmpColor = meshes[0].color;
        return tmpColor;
    }

    void myColor(aiColor3D newColor) {
        meshes[0].setColor(newColor);
        //meshes[0].color = newColor;
        //std::cout << "new red: " << 
    }

    // draws the model into a given texture, and thus all its meshes
    // 2d enviroments interoperability. Just to work with the correct shader and for full obj models with an mtl.
    // Usa el shader Color
    void DrawIntoTextureImport(Shader& shader)
    {
        //std::cout << "Meshes size: " + std::to_string(meshes.size()) << std::endl;
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].DrawIntoTextureImport(shader);
        }
    }

    // draws the model into a given texture, and thus all its meshes
    // 2d enviroments interoperability. Just to work with the correct shader and for custom obj models with an mtl.
    // Usa el shader TextColor
    void DrawIntoTextureCustom(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].DrawIntoTextureCustom(shader);
        }
    }

    // guarda el identificador del objeto en la informacion del frame buffer activado, de forma que se pueda
    // usar esta informacio cuando el usuario hace click sobre la pantalla, e identificar el objeto que se encuentra seleccionando
    // not working
    void DrawObjectsIdPixel(Shader& shader, aiColor3D idColor) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].DrawObjectsIdPixel(shader, idColor);
        }
    }

    // Constituye una malla dados vertices de triangulos, indices, texturas (calcula las normales al plano)
    void pushMesh(vector<SimpleVertex> vertices, aiColor3D color, vector<unsigned int> indexes)
    {
        // desacoplamos variables en caso de que se encuentren acopladas
        /*textures_loaded.clear();
        meshes.clear();
        directory = "";*/
         
        // loop through each 3 vertex
        // un vertice es una coordenada en 3D
        //std::vector<unsigned int> index;
        //for (unsigned int i = 0; i < vertices.size(); i++)
        //{
        //    // first triangle vertex
        //    std::string xPos1 = std::to_string(vertices[i].Position.x);
        //    std::string yPos1 = std::to_string(vertices[i].Position.y);
        //    std::string zPos1 = std::to_string(vertices[i].Position.z);
        //    // second triangle vertex
        //    std::string xPos2 = std::to_string(vertices[i + 1].Position.x);
        //    std::string yPos2 = std::to_string(vertices[i + 1].Position.y);
        //    std::string zPos2 = std::to_string(vertices[i + 1].Position.z);
        //    // third triangle vertex
        //    std::string xPos3 = std::to_string(vertices[i + 2].Position.x);
        //    std::string yPos3 = std::to_string(vertices[i + 2].Position.y);
        //    std::string zPos3 = std::to_string(vertices[i + 2].Position.z);

        //    // print
        //    std::string result = "(" + xPos1 + ", " + yPos1 + ", " + zPos1 + "), " +
        //        "(" + xPos2 + ", " + yPos2 + ", " + zPos2 + "), " +
        //        "(" + xPos3 + ", " + yPos3 + ", " + zPos3 + "), ";

        //    std::cout << "points: " << result << std::endl;

        //    index.push_back(i);
        //    index.push_back(i+1);
        //    index.push_back(i+2);

        //    // next
        //    i = i + 2;
        //}

        Mesh mesh = Mesh(vertices, indexes, color);

        meshes.push_back(mesh);

    }

    // Obtiene una aiScene para poder exportar.
    // Esta disenada para exportar solamente modelos customizados
    aiScene GetExportScene() {
        int numMeshes = meshes.size();
        //std::cout << "number of meshes in scene: " + std::to_string(numMeshes) << std::endl;
        aiMesh** myMeshes = new aiMesh* [numMeshes];
        aiMaterial** myMaterials = new aiMaterial * [numMeshes];
        unsigned int* meshesIndex = new unsigned int[numMeshes];
        for (int i = 0; i < numMeshes; i++) {
            // create vertex
            int numVertex = meshes[i].simpleVertices.size();
            //std::cout << "number of vertex in mesh: " + std::to_string(numVertex) << std::endl;
            aiVector3D* tmpVertices = new aiVector3D[numVertex];
            for (int j = 0; j < numVertex; j++) {
                ai_real xPos(meshes[i].simpleVertices[j].Position.x);
                ai_real yPos(meshes[i].simpleVertices[j].Position.y);
                ai_real zPos(meshes[i].simpleVertices[j].Position.z);

                //std::cout << "Point: " + std::to_string(xPos) + " " + std::to_string(yPos) + " " + std::to_string(zPos) << std::endl;
                aiVector3D tmpVect(xPos, yPos, zPos);
                tmpVertices[j] = tmpVect;
            }
            // create faces
            int numFaces = meshes[i].indices.size() / 3;
            //std::cout << "number of faces in mesh: " + std::to_string(numFaces) << std::endl;
            aiFace* faces = new aiFace[numFaces];
            for (int j = 0; j < numFaces; j++) {
                faces[j].mNumIndices = 3;       // in custom models we are always working with trianges
                faces[j].mIndices = new unsigned [3] {
                    meshes[i].indices[(j * 3) + 0],
                    meshes[i].indices[(j * 3) + 1],
                    meshes[i].indices[(j * 3) + 2]
                };
                //std::cout << "Face " + std::to_string(j) + ": " + std::to_string(meshes[i].indices[(j * 3) + 0]) + "," + std::to_string(meshes[i].indices[(j * 3) + 1]) + "," + std::to_string(meshes[i].indices[(j * 3) + 2]) << std::endl;
            }
            // create meshes
            aiMesh* mesh = new aiMesh();
            mesh->mNumVertices = numVertex;
            mesh->mVertices = tmpVertices;
            mesh->mNumFaces = numFaces;
            mesh->mFaces = faces;
            mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
            mesh->mMaterialIndex = 0;
            myMeshes[i] = mesh;         // add mesh to array of meshes
            meshesIndex[i] = i;         // add index of mesh to array of mesh indexes

            // a valid material is needed, even if its empty
            aiMaterial* material = new aiMaterial();
            aiColor3D color = aiColor3D(meshes[i].color.r, meshes[i].color.g, meshes[i].color.b);
            material->AddProperty<aiColor3D>(&color, 1, AI_MATKEY_COLOR_DIFFUSE);
            myMaterials[i] = material;      // add material to array of materials
        }

        // a root node with the mesh list is needed; if you have multiple meshes, this must match.
        aiNode* root = new aiNode();
        root->mNumMeshes = numMeshes;
        root->mMeshes = meshesIndex;
        // pack mesh(es), material, and root node into a new minimal aiScene
        aiScene* out = new aiScene();
        out->mNumMeshes = numMeshes;
        out->mMeshes = myMeshes;
        out->mNumMaterials = numMeshes;             // each mesh has only one material which in turn has one color
        out->mMaterials = myMaterials;
        out->mRootNode = root;
        out->mMetaData = new aiMetadata();

        return *out;
        
        //Assimp::Exporter mAiExporter;

        //const char* basePath = "./modelo3d/custom/";
        //const char* modelExt = ".obj";
        //size_t totalPathLng = strlen(basePath) + strlen(modelName) + strlen(modelExt);
        //char* myPath = new char[totalPathLng];
        //strcpy(myPath, basePath);
        //strcat(myPath, modelName);
        //strcat(myPath, modelExt);
        //if (mAiExporter.Export(out, "obj", myPath) != AI_SUCCESS) {
        //    cerr << mAiExporter.GetErrorString() << endl;
        //    std::cout << mAiExporter.GetErrorString() << std::endl;
        //}

        //// deleting the scene will also take care of the vertices, faces, meshes, materials, nodes, etc.
        //delete out;
    }

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path, bool processCustom)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene, processCustom);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene, bool processCustom)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            if (processCustom)
            {
                meshes.push_back(processMeshCustom(mesh, scene));
            }
            else
            {
                meshes.push_back(processMesh(mesh, scene));
            }
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, processCustom);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    Mesh processMeshCustom(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<SimpleVertex> vertices;
        vector<unsigned int> indices;
        aiColor3D color;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            SimpleVertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            //// texture coordinates
            //if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            //{
            //    glm::vec2 vec;
            //    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            //    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            //    vec.x = mesh->mTextureCoords[0][i].x;
            //    vec.y = mesh->mTextureCoords[0][i].y;
            //    vertex.TexCoords = vec;
            //}
            //else
            //    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        //// 1. diffuse maps
        //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //// 2. specular maps
        //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //// 3. normal maps
        //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //// 4. height maps
        //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // procesamos colores
        //"AI_MATKEY_COLOR_DIFFUSE" -> "$clr.diffuse"
        color = loadMaterialColors(material, "$clr.diffuse");

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, color);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }

    //  checks if there are colors of a given type and loads the colors if they're not loaded yet.
    // the required info is returned as a aiColor
    aiColor3D loadMaterialColors(aiMaterial* mat, const char* type)
    {
        //std::cout << mat->GetName().C_Str() << std::endl;
        aiColor3D color;
        //aiReturn ret = mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        aiReturn ret = mat->Get(type, 0,0,color);
        //std::cout << "Result code - load material color: " + ret << std::endl;
        //std:string ans = std::to_string(color.b) + " " + std::to_string(color.g) + " " + std::to_string(color.r);
        //std::cout << ans << std::endl;

        return color;
    }
};


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif