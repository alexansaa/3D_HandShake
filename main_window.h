#include <shader/shader_m.h>
#ifndef FIRST_PAGE
#define FIRST_PAGE
namespace MainWindow {
	void ShowMainWindow();
	extern bool adding_line;
	extern bool opt_enable_grid;
	void ShowCanvas();
	void ShowTest();
	void ShowImport();
	//extern Shader ourShader;
	//extern unsigned int fbo;
	extern unsigned int VAO;
	extern unsigned int texture;
}
#endif