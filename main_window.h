#ifndef FIRST_PAGE
#define FIRST_PAGE
#include <shader/shader_m.h>
namespace MainWindow {
	void ShowMainWindow();
	extern bool adding_line;
	extern bool opt_enable_grid;
	void ShowCanvas();
	void ShowTest();
	void ShowImport();
	void ShowDraw();
	void ExportCustom();
	void AddMeshToStateFull();
	void ImportMeshToStateFull(const char* modelName, bool processCustom);
	void RenderTexture(unsigned int texture);
	//extern Shader ourShader;
	//extern unsigned int fbo;
	extern unsigned int VAO;
	extern unsigned int texture;
}
#endif