#include "impExp.h"
#include <assimp/Exporter.hpp>
#include "main.h"

void import_export::Importation(const char* modelObjPath, bool processCustom) {
	// importacion del modelo desde un obj que tien un mtl
	Model tmpModel(modelObjPath, processCustom);
	prog_input::isImporting = true;
	import_export::tmpModel = tmpModel;
}

void import_export::Importation(Model myModel) {
	prog_input::isImporting = true;
	import_export::tmpModel = myModel;
}

void import_export::ImportationAcepted() {
	prog_input::isImporting = false;
	prog_state::stateModels.push_back(import_export::model);
}

void import_export::ImportationCanceled() {
	prog_input::isImporting = false;
	Model emptyModel = Model();
	import_export::tmpModel = emptyModel;
	import_export::model = emptyModel;
}

void import_export::ImportationTraslation() {
	glm::vec3 tmpVec;
	tmpVec.x = prog_input::intersectionPoint.x;
	tmpVec.y = prog_input::intersectionPoint.y;
	tmpVec.z = prog_input::intersectionPoint.z;

	vector<Mesh> importMeshes = import_export::tmpModel.meshes;
	aiColor3D myColor = importMeshes[0].color;
	vector<unsigned int> indices = importMeshes[0].indices;

	vector<SimpleVertex> vertex;

	for (int i = 0; i < importMeshes[0].simpleVertices.size(); i++) {
		SimpleVertex tmpV;
		glm::vec3 tmpN;
		tmpN.x = importMeshes[0].simpleVertices[i].Position.x + tmpVec.x;
		tmpN.y = importMeshes[0].simpleVertices[i].Position.y + tmpVec.y;
		tmpN.z = importMeshes[0].simpleVertices[i].Position.z + tmpVec.z;
		tmpV.Position = tmpN;
		vertex.push_back(tmpV);
	}

	Model transModel = Model();
	transModel.pushMesh(vertex, myColor, indices);
	import_export::model = transModel;
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