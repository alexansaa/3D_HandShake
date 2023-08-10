#include "main.h"
#include "inputAdd.h"
#include "Renderizador.h"
#include "main.h"

unsigned int render_state::RenderModelsVector(vector<Model> stateModel) {
    // Configuraciones de renderizado
    // ----------------------------------------------------
    // Create a texture for rendering
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
        std::cout << "TODO BIEN" << std::endl;
    }

    // liberamos los buffers para que el flujo de renderizacion de otros modulos pueda darse con normalidad
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

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

    //std::cout << "State models: " + std::to_string(stateModel.size()) << std::endl;

    // default configuration
    float textureMixRate = 0.8f;

    bool useTexture = true;
    bool useColor = true;

    for (int i = 0; i < stateModel.size(); i++) {
        stateModel[i].DrawIntoTexture(prog_state::renderShader, texture, framebuffer, rbo, textureMixRate, useTexture, useColor);
        //std::cout << "Model number: " + std::to_string(i) << std::endl;
    }

    return texture;
}

