#ifndef PROGRAM_INPUT
#define PROGRAM_INPUT
#include <GLFW/glfw3.h>

namespace prog_input {
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
	void mouse_click_callback(GLFWwindow* window, int mouseBtn, int btnAction, int modifierKeys);
	void resetState();
	extern bool isDragging;
	extern bool isInputting;
	extern bool isDrawing;
	extern bool isImporting;
	extern double prevXpos, prevYpos;
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;
	extern float deltaTime;
	extern float lastFrame;
	extern glm::vec3 intersectionPoint;
}
#endif