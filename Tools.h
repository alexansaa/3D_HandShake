#ifndef TOOLS
#define TOOLS

namespace GuiTools {
	//estado de ventanas de herramientas
	extern bool show_window_model;
	extern bool show_window_color;
	extern bool show_window_effects;
	extern bool show_window_shape;
	extern bool show_window_AboutUs;
	void ShowModelWindow(bool* p_open);
	void ShowColorWindow(bool* p_open);
	void ShowEffectsWindow(bool* p_open);
	void ShowShapeWindow(bool* p_open);
	void ShowAboutUsWindow(bool* p_open);


	void BarraHerramientas();
	static void ShowMenuFile();
	
}

#endif