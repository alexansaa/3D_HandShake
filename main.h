#ifndef PROGRAM_STATE
#define PROGRAM_STATE
#include <camera/camera.h>
#include <shader/shader.h>
#include "model.h"
namespace prog_state {
	extern Camera camera;
	extern Shader shader;
	extern Model model;
}
#endif