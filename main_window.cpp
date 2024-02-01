#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "main_window.h"
#include "Tools.h"
#include "main.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <math.h>
#include "model.h"

#include <shader/shader_m.h>

#include "main.h"
#include "camera.h"
#include "inputAdd.h"
#include "Renderizador.h"
#include "impExp.h"

#include <assimp/Exporter.hpp>
#include <assimp/GenericProperty.h>

using namespace Assimp;

//static Shader ourShader;
//unsigned int fbo;
unsigned int VAO;
unsigned int texture;

namespace MainWindow {
    static ImVector<ImVec2> points;
    static ImVec2 scrolling(0.0f, 0.0f);
    bool adding_line = false;
    bool opt_enable_grid = true;
    void ShowMainWindow() {
        static bool full_size_open = true;
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        static bool use_work_area = true;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        if (ImGui::Begin("Example: Fullscreen window", &full_size_open, flags))
        {
            GuiTools::BarraHerramientas();
            //ImGui::Text("Texto de ejemplo para la ventana");
            //ShowCanvas();                 // muestra el canvas de Dear Imgui
            //ShowTest();                   // usa vertices hardcoded para poder renderizar objetos con 2 diferentes texturas, sobre una textura requerida para renderizar en el 2D de Dear Imgui. Ocupa el shader textColor
            // --------------
            //ImportMeshToStateFull("Low-Poly Plant_", false);      // se requiere para hacer el trabajo de importacion a vector<Model> variable de estado de programa
            //ShowImport();                 // muestra la imagen renderizada importada del path al modelo obj con su respectivo mtl. Ocupa el Shader Color
            // --------------
            //ShowDraw();                   // muestra como renderizar en el ambiente 2D de Dear Imgui usando la nueva arquitectura. Con vertices customizados. Usa shader textcolor
            //ShowTetraedro();
            //ShowHexaedro();
            //ShowOctaedro();
            //ShowIcosaedro();
            //ShowEsfera();
            //ShowDodecaedro();
            //ExportCustom();               // exporta una escena .obj junto con su .mtl a la carpeta de modelos 3d del programa. Usa Mesh para configurar la escena. Usa shader textcolor

            // ejemplo que agrega un Modelo al vector de modelos del estado del programa para luego renderizar todos los modelos del vector de modelos
            // usa shader textColor
            //AddMeshToStateFull();
            //unsigned int myTexture = render_state::RenderModelsVector(prog_state::stateModels);
            //RenderTexture(myTexture);

            // ejemplo que importa un modelo al vector de modelos del estado del programa para luego renderizar todos los modelos del vector de modelos
            // usa shader textColor
            //ImportMeshToStateFull("triangleplusone", true);
            //unsigned int myTexture = render_state::RenderModelsVector(prog_state::stateModels);
            //RenderTexture(myTexture);

            // Metodo que renderiza los modelos del arreglo de modelos global
            glDeleteTextures(1, &prog_state::mainTexture);
            vector<Model> modelsToRender = prog_state::stateModels;
            if (prog_input::isDrawing) {
                //std::cout << "progam is drawing" << std::endl;
                modelsToRender.push_back(prog_state::tmpModel);
            }
            if (prog_input::isImporting) {
                modelsToRender.push_back(import_export::model);
            }

            prog_state::mainTexture = render_state::RenderModelsVector(modelsToRender);
            render_state::RenderTexture(prog_state::mainTexture);
            //glFinish();
            
        }
        ImGui::End();
    }

    void ShowCanvas() {
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

        // Draw border and background color
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

        // This will catch our interactions
        ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered(); // Hovered
        const bool is_active = ImGui::IsItemActive();   // Held
        const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

        // Add first and second point
        if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            points.push_back(mouse_pos_in_canvas);
            points.push_back(mouse_pos_in_canvas);
            adding_line = true;
        }
        if (adding_line)
        {
            points.back() = mouse_pos_in_canvas;
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                adding_line = false;
        }

        // Draw grid + all lines in the canvas
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);
        if (opt_enable_grid)
        {
            const float GRID_STEP = 64.0f;
            for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
            for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
        }
        for (int n = 0; n < points.Size; n += 2)
            draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
        draw_list->PopClipRect();
    }

    void ShowTest() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // load and create a texture 
    // -------------------------
        unsigned int texture1, texture2;
        // texture 1
        // ---------
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        //unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
        unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        // texture 2
        // ---------
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        //data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
        data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        // definimos shader
        //Shader ourShader("text.vs", "test.fs");
        //Shader ourShader("Shaders/7.4.camera.vs", "Shaders/7.4.camera.fs");
        
        //ourShader.use();
        //ourShader.setInt("texture1", 0);
        //ourShader.setInt("texture2", 1);

        prog_state::renderShader.use();
        prog_state::renderShader.setInt("texture1", 0);
        prog_state::renderShader.setInt("texture2", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(prog_state::camera.Zoom), (float)prog_input::SCR_WIDTH / (float)prog_input::SCR_HEIGHT, 0.1f, 100.0f);
        prog_state::renderShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f);
        view = prog_state::camera.GetViewMatrix();
        prog_state::renderShader.setMat4("view", view);

        // shader settings
        prog_state::renderShader.setBool("useTexture", true);
        prog_state::renderShader.setBool("useColor", false);
        prog_state::renderShader.setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        prog_state::renderShader.setFloat("textureMixFactor", 0.2);

        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, prog_input::SCR_WIDTH, prog_input::SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
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

        glBindVertexArray(VAO);

        // first pass
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

        // render boxes

        for (unsigned int i = 0; i < 10; i++)
        {

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            prog_state::renderShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &texture1);
        glDeleteTextures(1, &texture2);
        //glDeleteTextures(1, &texture);
        //glDeleteFramebuffers(GL_FRAMEBUFFER, &framebuffer);
        glDeleteRenderbuffers(1, &rbo);

        // always good practice to set everything back to defaults once configured.
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void ShowImport() {
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
            std::cout << "TODO BIEN" << std::endl;
        }

        // don't forget to enable shader before setting uniforms
        // para que este ejemplo funcione correctamente, se requiere de un shader que acepte especificamente el nombre de
        // la textura que se pasa desde el modelo obj. Es por esa razon que un modelo obj siempre estara acompanado de un mtl y
        // un shader en especifico que funcione bien para el proposito de renderizacion
        // en este caso funciona bien con el render "colors" que corresponde al shader "1.model_loading"
        // el cual acepta una textura y las variables correspondientes al renderizado de la camara
        // este shader no es util para mezclar texturas, sino para renderizar mallas las cuales tengan una sola textura cada malla
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

        // Variables de renderizado
        //float textureMixRate = 0.5f;
        //bool useTexture = true;
        //bool useColor = false;

        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // renderizamos en textura
        import_export::model.DrawIntoTextureImport(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();

        //delete objects
        //glDeleteTextures(1, &texture);
        //glDeleteFramebuffers(1, &framebuffer);
        //glDeleteRenderbuffers(1, &rbo);
    }

    void ShowDraw() {
        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE3);
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
            std::cout << "TODO BIEN" << std::endl;
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

        // definimos nuestros vertices
        vector<SimpleVertex> myVertices;

        SimpleVertex v1;
        glm::vec3 vec1;
        vec1.x = -0.5f;
        vec1.y = -0.5f;
        vec1.z = -0.5f;
        v1.Position = vec1;

        SimpleVertex v2;
        glm::vec3 vec2;
        vec2.x = 0.5f;
        vec2.y = -0.5f;
        vec2.z = -0.5f;
        v2.Position = vec2;

        SimpleVertex v3;
        glm::vec3 vec3;
        vec3.x = 0.5f;
        vec3.y = 0.5f;
        vec3.z = -0.5f;
        v3.Position = vec3;

        SimpleVertex v4;
        glm::vec3 vec4;
        vec4.x = 0.5f;
        vec4.y = -0.5f;
        vec4.z = -1.0f;
        v4.Position = vec4;

        myVertices.push_back(v1);
        myVertices.push_back(v2);
        myVertices.push_back(v3);
        myVertices.push_back(v4);

        // definimos un color
        aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

        // definimos indices de orden de renderizado
        vector<unsigned int> myIndexes;
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        // Con 3 vertices se puede crear 1 solo triangulo
        // cuando agrego un vertice mas, se puede construir hasta 3 triangulos extras
        // dependiendo de la configuracion, se va a crear uno u otro triangulo.
        // aqui todas las posibilidades
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        myIndexes.push_back(3);
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(3);
        // este ultimo triangulo puede ser descartado del dibujo comentando estas lineas
        // es interesante que se agregen color a los vertices para que se pueda diferenciar entre
        // una figura solida abierta y una cerrada
        myIndexes.push_back(2);
        myIndexes.push_back(0);
        myIndexes.push_back(3);

        Model myModel = Model();
        myModel.pushMesh(myVertices, myColor, myIndexes);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderizamos en textura
        myModel.DrawIntoTextureCustom(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    void ShowTetraedro() {
        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE3);
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
            std::cout << "TODO BIEN" << std::endl;
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

        // definimos nuestro modelo
        Model MyTetraedro = import_export::Tetraedro();
        
        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderizamos en textura
        MyTetraedro.DrawIntoTextureCustom(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    void ShowHexaedro() {
        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE3);
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
            std::cout << "TODO BIEN" << std::endl;
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

        // definimos nuestro modelo
        Model MyHexaedro = import_export::Hexaedro();

        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderizamos en textura
        MyHexaedro.DrawIntoTextureCustom(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    void ShowOctaedro() {
        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE3);
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
            std::cout << "TODO BIEN" << std::endl;
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

        // definimos nuestro modelo
        Model MyOctaedro = import_export::Octaedro();

        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderizamos en textura
        MyOctaedro.DrawIntoTextureCustom(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    void ShowIcosaedro() {
        // Create a texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE3);
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
            std::cout << "TODO BIEN" << std::endl;
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

        // definimos nuestro modelo
        Model MyIcosaedro = import_export::Icosaedro();

        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.5f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderizamos en textura
        MyIcosaedro.DrawIntoTextureCustom(prog_state::renderShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    void ExportCustom() {
        // definimos nuestros vertices
        vector<SimpleVertex> myVertices;

        SimpleVertex v1;
        glm::vec3 vec1;
        vec1.x = -0.5f;
        vec1.y = -0.5f;
        vec1.z = -0.5f;
        v1.Position = vec1;

        SimpleVertex v2;
        glm::vec3 vec2;
        vec2.x = 0.5f;
        vec2.y = -0.5f;
        vec2.z = -0.5f;
        v2.Position = vec2;

        SimpleVertex v3;
        glm::vec3 vec3;
        vec3.x = 0.5f;
        vec3.y = 0.5f;
        vec3.z = -0.5f;
        v3.Position = vec3;

        SimpleVertex v4;
        glm::vec3 vec4;
        vec4.x = 0.5f;
        vec4.y = -0.5f;
        vec4.z = -1.0f;
        v4.Position = vec4;

        myVertices.push_back(v1);
        myVertices.push_back(v2);
        myVertices.push_back(v3);
        myVertices.push_back(v4);

        // definimos un color
        aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

        // definimos indices de orden de renderizado
        vector<unsigned int> myIndexes;
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        myIndexes.push_back(3);
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(3);
        //myIndexes.push_back(2);
        //myIndexes.push_back(0);
        //myIndexes.push_back(3);

        Model myModel = Model();
        myModel.pushMesh(myVertices, myColor, myIndexes);

        aiScene myScene = myModel.GetExportScene();


        //// --------------------------------------------------

        //// create vertices and faces, then pack into an aiMesh

        //aiVector3D* vertices = new aiVector3D[3]{          // deleted: mesh.h:758
        //    { -1, -1, 0 },
        //    { 0, 1, 0 },
        //    { 1, -1, 0 }
        //};

        ////aiVector3D* normal = new aiVector3D[1]{
        ////    {0,0,-1}
        ////};

        //aiFace* faces = new aiFace[1];                      // deleted: mesh.h:784
        //faces[0].mNumIndices = 3;
        //faces[0].mIndices = new unsigned [3] { 0, 1, 2 };    // deleted: mesh.h:149

        //aiMesh* mesh = new aiMesh();                        // deleted: Version.cpp:150
        //mesh->mNumVertices = 3;
        //mesh->mVertices = vertices;
        //mesh->mNumFaces = 1;
        //mesh->mFaces = faces;
        //mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE; // workaround, issue #3778
        //mesh->mMaterialIndex = 0;

        //// a valid material is needed, even if its empty

        //aiMaterial* material = new aiMaterial();            // deleted: Version.cpp:155
        //aiColor3D color = aiColor3D(0.3f,0.5f,0.3f);
        //material->AddProperty<aiColor3D>(&color, 1, AI_MATKEY_COLOR_DIFFUSE);

        //// a root node with the mesh list is needed; if you have multiple meshes, this must match.

        //aiNode* root = new aiNode();                        // deleted: Version.cpp:143
        //root->mNumMeshes = 1;
        //root->mMeshes = new unsigned [1] { 0 };              // deleted: scene.cpp:77

        //// pack mesh(es), material, and root node into a new minimal aiScene

        //aiScene* out = new aiScene();                       // deleted: by us after use
        //out->mNumMeshes = 1;
        //out->mMeshes = new aiMesh * [1] { mesh };            // deleted: Version.cpp:151
        //out->mNumMaterials = 1;
        //out->mMaterials = new aiMaterial * [1] { material }; // deleted: Version.cpp:158
        //out->mRootNode = root;
        //out->mMetaData = new aiMetadata(); // workaround, issue #3781

        Assimp::Exporter mAiExporter;

        if (mAiExporter.Export(&myScene, "obj", "./modelo3d/custom/triangleplusone.obj") != AI_SUCCESS) {
            cerr << mAiExporter.GetErrorString() << endl;
            std::cout << mAiExporter.GetErrorString() << std::endl;
        }
            
        // in case of returnint the aiScene from another function, there should not be worries about dellocationg memmory as memory should me managed by c++
        // deleting the scene will also take care of the vertices, faces, meshes, materials, nodes, etc.
        //delete &myScene;


        //ExportProperties* properties = new ExportProperties;
        //properties->SetPropertyBool(AI_CONFIG_EXPORT_POINT_CLOUDS, true);
        ////mAiExporter.Export(&scene, "stl", "./modelo3d/testExport.stl", 0, properties);
        //mAiExporter.Export(&scene, "obj", "./modelo3d/testExport.obj");

        //delete properties;
    }

    void AddMeshToStateFull() {
        // definimos nuestros vertices
        vector<SimpleVertex> myVertices;

        SimpleVertex v1;
        glm::vec3 vec1;
        vec1.x = -0.5f;
        vec1.y = -0.5f;
        vec1.z = -0.5f;
        v1.Position = vec1;

        SimpleVertex v2;
        glm::vec3 vec2;
        vec2.x = 0.5f;
        vec2.y = -0.5f;
        vec2.z = -0.5f;
        v2.Position = vec2;

        SimpleVertex v3;
        glm::vec3 vec3;
        vec3.x = 0.5f;
        vec3.y = 0.5f;
        vec3.z = -0.5f;
        v3.Position = vec3;

        SimpleVertex v4;
        glm::vec3 vec4;
        vec4.x = 0.5f;
        vec4.y = -0.5f;
        vec4.z = -1.0f;
        v4.Position = vec4;

        myVertices.push_back(v1);
        myVertices.push_back(v2);
        myVertices.push_back(v3);
        myVertices.push_back(v4);

        // definimos un color
        aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

        // definimos indices de orden de renderizado
        vector<unsigned int> myIndexes;
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        myIndexes.push_back(1);
        myIndexes.push_back(2);
        myIndexes.push_back(3);
        myIndexes.push_back(0);
        myIndexes.push_back(1);
        myIndexes.push_back(3);
        //myIndexes.push_back(2);
        //myIndexes.push_back(0);
        //myIndexes.push_back(3);

        Model myModel = Model();
        myModel.pushMesh(myVertices, myColor, myIndexes);

        if (prog_state::stateModels.size() < 1) {
            prog_state::stateModels.push_back(myModel);
        }

        

        //float textureMixRate = 0.8f;

        //bool useTexture = true;
        //bool useColor = true;
    }

    // dependiendo de donde esta el modelo, se debe cambiar basePath
    void ImportMeshToStateFull(const char* modelName, bool processCustom) {
        const char* basePath = "./modelo3d/custom/";
        const char* modelExt = ".obj";
        size_t totalPathLng = strlen(basePath) + strlen(modelName) + strlen(modelExt) + 1;
        char* myPath = new char[totalPathLng];
        strncpy_s(myPath, totalPathLng, basePath, totalPathLng - 1);
        strncat_s(myPath, totalPathLng, modelName, totalPathLng - strlen(myPath) - 1);
        strncat_s(myPath, totalPathLng, modelExt, totalPathLng - strlen(myPath));
        std::cout << "Path: " << myPath << std::endl;

        import_export::Importation(myPath, processCustom);

        if (prog_state::stateModels.size() < 1) {
            prog_state::stateModels.push_back(import_export::model);
        }
        
    }

    // funcion usada para renderizar una textura sobre ambiente 2D de Dear Imgui
    void RenderTexture(unsigned int texture) {
        //Al final si todo salio bien te quedas con una textura renderizada. en este caso "texture"
        ImGui::BeginChild("GameRender");

        //ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImVec2 wsize = ImGui::GetWindowSize();

        ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }
}