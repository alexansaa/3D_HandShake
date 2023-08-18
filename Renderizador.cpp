#include "main.h"
#include "inputAdd.h"
#include "Renderizador.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "inputAdd.h"

#include <cmath>
#include "Globals.h"

vector<SimpleVertex> render_state::inputModelVertices;
vector<unsigned int> render_state::inputModelIndexes;
aiColor3D render_state::inputModelColor(0.3f, 0.7f, 0.5f);

unsigned int render_state::RenderModelsVector(vector<Model> modelArray) {
    // Configuraciones de renderizado
    // ----------------------------------------------------
    // Create a texture for rendering
    unsigned int texture;
    glGenTextures(1, &texture);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    // set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering paramteres
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create a framebuffer object (FBO)
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    //Create render buffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, prog_input::SCR_WIDTH, prog_input::SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check si el framebuffer esta completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
    }
    else
    {
        //std::cout << "RenderModelsVector Texture is OK!" << std::endl;
    }

    // don't forget to enable shader before setting uniforms
    prog_state::renderShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(prog_state::camera.Zoom), (float)prog_input::SCR_WIDTH / (float)prog_input::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = prog_state::camera.GetViewMatrix();
    prog_state::renderShader.setMat4("projection", projection);
    prog_state::renderShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    prog_state::renderShader.setMat4("model", model);
    glClearColor(globalBackgroundColor.x, globalBackgroundColor.y, globalBackgroundColor.z, globalBackgroundColor.w); //color de fondo
    //glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // renderizamos modelos sobre textura
    //std::cout << "Number of models: " << modelArray.size() << std::endl;
    for (int i = 0; i < modelArray.size(); i++) {
        modelArray[i].DrawIntoTextureCustom(prog_state::renderShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

    return texture;
}
void render_state::RenderTexture(unsigned int texture) {
    //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
    ImGui::BeginChild("GameRender");

    //ImVec2 wsize = ImGui::GetContentRegionAvail();
    ImVec2 wsize = ImGui::GetWindowSize();

    ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::EndChild();
}

// Se tiene que llamar dos veces a este metodo para que un ciclo de "dibujo" se complete:
// 1) Setea booleado "isInputting" a true y al finalizar los llamados con true, o sea mientras realiza el dibujo,
//          setea booleano a false pero se llama una vez mas para finalizar el mismo ciclo de dibujo con un false
//          y resetea los estados
void render_state::InputModelCreator() {
    Model myModel = Model();
    if (prog_input::isInputting) {
        SimpleVertex imputVertex;
        glm::vec3 tmpVec;
        tmpVec.x = prog_input::intersectionPoint.x;
        tmpVec.y = prog_input::intersectionPoint.y;
        tmpVec.z = prog_input::intersectionPoint.z;
        std::cout << "Point: " << tmpVec.x << "," << tmpVec.y << "," << tmpVec.z << std::endl;
        imputVertex.Position = tmpVec;
        render_state::inputModelVertices.push_back(imputVertex);

        // manejo el indice de renderizacion
        int inputVertexSize = render_state::inputModelVertices.size();
        if (inputVertexSize == 3) {
            render_state::inputModelIndexes.push_back(0);
            render_state::inputModelIndexes.push_back(1);
            render_state::inputModelIndexes.push_back(2);
        } else if (inputVertexSize > 3) {
            // Aqui podemos cambiar el algoritmo de seleccion de nodos para creacion de nuevos triangulos
            // Se puede implementar una funcion que pushe los indices del vertex ingresado junto con los otros
            // dos vertices mas cercanos, realizando una resta del nodo ingresado (en cada coordenada) menos cada
            // nodo dentro del vector de nodos ya ingresados. Si se sigue la trazabilidad de los indices, se puede ingresar
            // vertices que se encuentran mas cercanos al ingresado actualmente
            unsigned int newIndex = render_state::inputModelIndexes.size();
            render_state::inputModelIndexes.push_back(newIndex);
            render_state::inputModelIndexes.push_back(newIndex - 2);
            render_state::inputModelIndexes.push_back(newIndex - 1);
        }
    }
    else
    {
        if (render_state::inputModelVertices.size() > 0) {
            myModel.pushMesh(render_state::inputModelVertices, render_state::inputModelColor, render_state::inputModelIndexes);
            std::cout << "Creando modelo" << std::endl;
            prog_state::stateModels.push_back(myModel);
            std::cout << "Pusheando modelo" << std::endl;
        }

        //reseteo los estados
        vector<unsigned int> newVectorIndex;
        render_state::inputModelIndexes = newVectorIndex;
        vector<SimpleVertex> newVectorVertex;
        render_state::inputModelVertices = newVectorVertex;
        std::cout << "Reseteo de variables" << std::endl;
    }
    // verifico espacio de vertices ingresados para poder renderizar o descartar
    if (render_state::inputModelVertices.size() > 2) {
        // preparo modelo
        myModel.pushMesh(render_state::inputModelVertices, render_state::inputModelColor, render_state::inputModelIndexes);
        std::cout << "Mesh pushed to render..." << std::endl;
        // asigno a la variable global del Modelo "siendo dibujado"
        prog_state::tmpModel = myModel;
        prog_input::isDrawing = true;
    }
    else {
        prog_input::isDrawing = false;
        prog_state::tmpModel = myModel;
    }
}