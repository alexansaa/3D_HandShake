#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <shader/shader.h>

#include "main.h"
#include "inputAdd.h"
#include "camera.h"
#include "main_window.h"
#include "Tools.h"
#include "impExp.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// camera
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 15.0f;
Camera prog_state::camera(glm::vec3(cameraX, cameraY, cameraZ));

// shader
Shader prog_state::renderShader;
Shader prog_state::pickingShader;

// importer object
Model import_export::model;

// model state
unsigned int prog_state::mainTexture;
vector<Model> prog_state::stateModels;
Model prog_state::tmpModel;

// constants defintion
extern const char* prog_state::colorVS = "./Shaders/1.model_loading.vs";
extern const char* prog_state::colorFS = "./Shaders/1.model_loading.fs";
extern const char* prog_state::textureVS = "./Shaders/7.4.camera.vs";
extern const char* prog_state::textureFS = "./Shaders/7.4.camera.fs";
extern const char* prog_state::textColorVS = "./Shaders/test.vs";
extern const char* prog_state::textColorFS = "./Shaders/test.fs";
extern const char* prog_state::pickingVS = "./Shaders/picking.vs";
extern const char* prog_state::pickingFS = "./Shaders/picking.fs";

// global variables
ImVec4 prog_state::globalBackgroudColor = ImVec4(0.5f, 0.3f, 0.5f, 1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(prog_input::SCR_WIDTH, prog_input::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, prog_input::mouse_callback);
    glfwSetMouseButtonCallback(window, prog_input::mouse_click_callback);
    glfwSetScrollCallback(window, prog_input::scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    prog_state::renderShader = Shader(prog_state::textColorVS, prog_state::textColorFS);
    prog_state::pickingShader = Shader(prog_state::pickingVS, prog_state::pickingFS);
    //prog_state::renderShader = Shader(prog_state::colorVS, prog_state::colorFS);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        prog_input::deltaTime = currentFrame - prog_input::lastFrame;
        prog_input::lastFrame = currentFrame;

        // input
        // -----
        prog_input::processInput(window);

        // Renderizamos Dear Imgui
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        MainWindow::ShowMainWindow();

        //Renderizado ventanas de modelos
        GuiTools::ShowModelWindow(&GuiTools::show_window_model);
        GuiTools::ShowColorWindow(&GuiTools::show_window_color);
        GuiTools::ShowEffectsWindow(&GuiTools::show_window_effects);
        GuiTools::ShowShapeWindow(&GuiTools::show_window_shape);
        GuiTools::ShowAboutUsWindow(&GuiTools::show_window_AboutUs);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}