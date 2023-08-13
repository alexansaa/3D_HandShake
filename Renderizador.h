#ifndef RENDER_STATE
#define RENDER_STATE
namespace render_state {
	unsigned int RenderModelsVector(vector<Model> stateModels);
	void RenderTexture(unsigned int texture);
	void InputModelCreator();
	extern vector<SimpleVertex> inputModelVertices;
	extern vector<unsigned int> inputModelIndexes;
	extern aiColor3D inputModelColor;
}
#endif
