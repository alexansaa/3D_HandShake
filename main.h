#ifndef PROGRAM_STATE
#define PROGRAM_STATE
#include "camera.h"
#include "model.h"
#include <shader/shader.h>

namespace prog_state {
	extern Camera camera;
	extern Shader renderShader;
	extern vector<Mesh> stateMeshes;
	extern Mesh tmpMesh;
	extern const char* colorVS;
	extern const char* colorFS;
	extern const char* textureVS;
	extern const char* textureFS;
	extern const char* textColorVS;
	extern const char* textColorFS;
}
#endif