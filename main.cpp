#include "main_window.h"
#include <tools/Tools.h>
#include "main.h"
#include <camera/camera.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_click_callback(GLFWwindow* window, int mouseBtn, int btnAction, int modifierKeys);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 15.0f;
Camera prog_state::camera(glm::vec3(cameraX, cameraY, cameraZ));

// 3d model shader
Shader prog_state::shader;

// sample model (solo ejemplo)
Model prog_state::model;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// mouse
bool isDragging = false;
double prevXpos, prevYpos;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    prog_state::shader = Shader("./Shaders/1.model_loading.vs", "./Shaders/1.model_loading.fs");
    prog_state::model = Model("modelo3d/Wolf_obj.obj");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

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
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        prog_state::camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (isDragging) {
        float deltaX = prevXpos - xposIn;
        float deltaY = yposIn - prevYpos;

        prog_state::camera.ProcessMouseMovement(deltaX, deltaY);

        prevXpos = xposIn;
        prevYpos = yposIn;
    }
    else
    {
        // normalizamos la posicion a [-1, 1]
        float nomrX = (2.0f * static_cast<float>(xposIn)) / SCR_WIDTH - 1.0f;
        float normY = 1.0f - (2.0f * static_cast<float>(yposIn)) / SCR_HEIGHT;

        // construimos la direccion del rayo en espacio de vista
        glm::vec4 rayClipSpace(nomrX, normY, -1.0f, 1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(prog_state::camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projectionMatrix = camera.GetProjectionMatrix();
        glm::mat4 projectionMatrix = projection;
        glm::vec4 rayViewSpace = glm::inverse(projectionMatrix) * rayClipSpace;
        rayViewSpace = glm::vec4(rayViewSpace.x, rayViewSpace.y, -1.0f, 0.0f);

        // tranforma la direccion del rayo a espacio de mundo
        glm::mat4 viewMatrix = prog_state::camera.GetViewMatrix();
        glm::vec4 rayWorldSpace = glm::inverse(viewMatrix) * rayViewSpace;
        glm::vec3 rayDirection(rayWorldSpace.x, rayWorldSpace.y, rayWorldSpace.z);
        rayDirection = glm::normalize(rayDirection);

        // Calculate the intersection point in world space
        float distanceFromCamera = 10.0f; // Adjust this distance as needed
        //glm::vec3 cameraPosition = glm::vec3(cameraX, cameraY, cameraZ);
        glm::vec3 cameraPosition = prog_state::camera.Position;
        glm::vec3 intersectionPoint = cameraPosition + rayDirection * distanceFromCamera;

        // Output the intersection point coordinates
        std::cout << "Intersection point: (" << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << ")" << std::endl;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    prog_state::camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// alex: cuando algun boton de mouse es presionado, esta funcion se llama
void mouse_click_callback(GLFWwindow* window, int mouseBtn, int btnAction, int modifierKeys) {
    switch (mouseBtn) {
    case GLFW_MOUSE_BUTTON_LEFT:
        std::cout << "left";
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (btnAction == GLFW_PRESS) {
            glfwGetCursorPos(window, &prevXpos, &prevYpos);
            std::cout << "x: " << prevXpos << " y: " << prevYpos << std::endl;
            isDragging = true;
        }
        else
        {
            isDragging = false;
        }
        break;
    default:
        std::cout << "no option btn";
    }
}