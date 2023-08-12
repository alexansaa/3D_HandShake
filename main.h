#ifndef PROGRAM_STATE
#define PROGRAM_STATE
#include "camera.h"
#include "model.h"
#include <shader/shader.h>

namespace prog_state {
	extern Camera camera;
	extern Shader renderShader;
	extern vector<Model> stateModels;
	extern Model tmpModel;
	extern const char* colorVS;
	extern const char* colorFS;
	extern const char* textureVS;
	extern const char* textureFS;
	extern const char* textColorVS;
	extern const char* textColorFS;
}
#endif