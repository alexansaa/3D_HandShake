#include "main.h"
#include "inputAdd.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include "Renderizador.h"

// variable definitions
bool prog_input::isDragging = false;
bool prog_input::isInputting = false;
bool prog_input::isDrawing = false;
double prog_input::prevXpos, prog_input::prevYpos;
unsigned int prog_input::SCR_WIDTH = 800;
unsigned int prog_input::SCR_HEIGHT = 600;
float prog_input::deltaTime = 0.0f;
float prog_input::lastFrame = 0.0f;
glm::vec3 prog_input::intersectionPoint;

void prog_input::resetState() {
    prog_input::isInputting = false;
    prog_input::isDrawing = false;
    prog_input::isDragging = false;
    vector<unsigned int> newVectorIndex;
    render_state::inputModelIndexes = newVectorIndex;
    vector<SimpleVertex> newVectorVertex;
    render_state::inputModelVertices = newVectorVertex;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void prog_input::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (prog_input::isDragging) {
        float deltaX = static_cast<float>(prog_input::prevXpos - xposIn);
        float deltaY = static_cast<float>(yposIn - prevYpos);

        prog_state::camera.ProcessMouseMovement(deltaX, deltaY);

        prog_input::prevXpos = xposIn;
        prog_input::prevYpos = yposIn;
    }
    else
    {
        // normalizamos la posicion a [-1, 1]
        float nomrX = (2.0f * static_cast<float>(xposIn)) / prog_input::SCR_WIDTH - 1.0f;
        float normY = 1.0f - (2.0f * static_cast<float>(yposIn)) / prog_input::SCR_HEIGHT;

        // construimos la direccion del rayo en espacio de vista
        glm::vec4 rayClipSpace(nomrX, normY, -1.0f, 1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(prog_state::camera.Zoom), (float)prog_input::SCR_WIDTH / (float)prog_input::SCR_HEIGHT, 0.1f, 100.0f);
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
        glm::vec3 cameraPosition = prog_state::camera.Position;
        prog_input::intersectionPoint = cameraPosition + rayDirection * distanceFromCamera; // obtenemos la posicion del mouse proyectado a la distancia fijada de la camara

        // Output the intersection point coordinates
        //std::cout << "Intersection point: (" << prog_input::intersectionPoint.x << ", " << prog_input::intersectionPoint.y << ", " << prog_input::intersectionPoint.z << ")" << std::endl;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void prog_input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    prog_state::camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// alex: cuando algun boton de mouse es presionado, esta funcion se llama
void prog_input::mouse_click_callback(GLFWwindow* window, int mouseBtn, int btnAction, int modifierKeys) {
    switch (mouseBtn) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (btnAction == GLFW_PRESS) {
            // verifico si se seleciona un objeto o el simple fondo o un boton del menu Dear Imgui
            // falta implementacion
            //render_state::HoverOverModelIdentifier(prog_state::stateModels);

            if (prog_input::isInputting) {
                std::cout << "inputing..." << std::endl;
                render_state::InputModelCreator();
            }
            else 
            {
                std::cout << "not inputing..." << std::endl;
            }
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (btnAction == GLFW_PRESS) {
            glfwGetCursorPos(window, &prog_input::prevXpos, &prog_input::prevYpos);
            std::cout << "x: " << prog_input::prevXpos << " y: " << prog_input::prevYpos << std::endl;
            prog_input::isDragging = true;
        }
        else
        {
            prog_input::isDragging = false;
        }
        break;
    default:
        std::cout << "no option btn";
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void prog_input::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //glfwSetWindowShouldClose(window, true);
        prog_input::resetState();
    }
        
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        render_state::InputModelCreator();
        std::cout << "Input has finnished!" << std::endl;
    }

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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        prog_input::isInputting = true;
    }
    else
    {
        prog_input::isInputting = false;
    }
        
}