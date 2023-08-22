#ifndef PROGRAM_STATE
#define PROGRAM_STATE
#include "camera.h"
#include "model.h"
#include <shader/shader.h>
#include <imgui/imgui.h>

namespace prog_state {
	extern Camera camera;
	extern Shader renderShader;
	extern Shader pickingShader;
	extern vector<Model> stateModels;
	extern Model tmpModel;
	extern const char* colorVS;
	extern const char* colorFS;
	extern const char* textureVS;
	extern const char* textureFS;
	extern const char* textColorVS;
	extern const char* textColorFS;
	extern const char* pickingVS;
	extern const char* pickingFS;
	extern ImVec4 globalBackgroudColor;
}
#endif