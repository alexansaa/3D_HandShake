#include "impExp.h"
#include <assimp/Exporter.hpp>

void import_export::Importation(const char* modelObjPath, bool processCustom) {
	// importacion del modelo desde un obj que tien un mtl
	Model tmpModel(modelObjPath, processCustom);
	import_export::model = tmpModel;
}

void import_export::Exportation(Model myModel, char* myName) {
	aiScene myExpScene = myModel.GetExportScene();
	Assimp::Exporter mAiExporter;

	// realizamos concatenacion de path final
	const char* root = "./modelo3d/custom/";
	const char* ext = ".obj";
	size_t totalLength = strlen(root) + strlen(myName) +  strlen(ext) + 1;

	char* fullpath = new char[totalLength];
	strcpy_s(fullpath, sizeof(root), root);
	strcpy_s(fullpath, sizeof(myName), myName);
	strcpy_s(fullpath, sizeof(ext), ext);
	//strcpy(fullpath, root);
	//strcpy(fullpath, myName);
	//strcpy(fullpath, ext);

	if (mAiExporter.Export(&myExpScene, "obj", fullpath) != AI_SUCCESS) {
		cerr << mAiExporter.GetErrorString() << endl;
		std::cout << mAiExporter.GetErrorString() << std::endl;
	}

	delete[] fullpath;
}

Model import_export::Tetraedro() {
	vector<SimpleVertex> myVertices;

	SimpleVertex v1;
	glm::vec3 vec1;
	vec1.x = 1.0f;
	vec1.y = 1.0f;
	vec1.z = 1.0f;
	v1.Position = vec1;

	SimpleVertex v2;
	glm::vec3 vec2;
	vec2.x = -1.0f;
	vec2.y = -1.0f;
	vec2.z = 1.0f;
	v2.Position = vec2;

	SimpleVertex v3;
	glm::vec3 vec3;
	vec3.x = -1.0f;
	vec3.y = 1.0f;
	vec3.z = -1.0f;
	v3.Position = vec3;

	SimpleVertex v4;
	glm::vec4 vec4;
	vec4.x = 1.0f;
	vec4.y = -1.0f;
	vec4.z = -1.0f;
	v4.Position = vec4;

	myVertices.push_back(v1);
	myVertices.push_back(v2);
	myVertices.push_back(v3);
	myVertices.push_back(v4);

	// definimos un color
	aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

	// definimos indices de orden de renderizado
	vector<unsigned int> myIndexes;
	myIndexes.push_back(0);
	myIndexes.push_back(1);
	myIndexes.push_back(2);

	myIndexes.push_back(0);
	myIndexes.push_back(3);
	myIndexes.push_back(2);

	myIndexes.push_back(3);
	myIndexes.push_back(2);
	myIndexes.push_back(1);

	myIndexes.push_back(0);
	myIndexes.push_back(2);
	myIndexes.push_back(3);

	Model myModel = Model();
	myModel.pushMesh(myVertices, myColor, myIndexes);

	return myModel;
}