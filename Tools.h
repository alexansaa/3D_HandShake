#ifndef TOOLS
#define TOOLS

namespace GuiTools {
	//estado de ventanas de herramientas
	extern bool show_window_model;
	extern bool show_window_color;
	extern bool show_window_effects;
	extern bool show_window_shape;
	extern bool show_window_objinfo;
	extern bool show_window_objinfo_colorSelect;
	extern bool show_window_objimport;
	extern bool show_window_objexp;
	extern bool show_window_AboutUs;

	extern int selectedModelIndex;

	void ShowModelWindow(bool* p_open);
	void ShowColorWindow(bool* p_open);
	void ShowEffectsWindow(bool* p_open);
	void ShowShapeWindow(bool* p_open);
	void ShowAboutUsWindow(bool* p_open);
	void ShowObjinfoWindow(bool* p_open, bool* c_open);
	void ShowObjexpWindow(bool* p_open);
	void ShowObjimportWindow(bool* p_open);


	void BarraHerramientas();
	static void ShowMenuFile();
	
}

#endif