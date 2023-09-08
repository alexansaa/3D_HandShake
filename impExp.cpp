#include "impExp.h"
#include <assimp/Exporter.hpp>
#include "main.h"

#include <string>
#include <iostream>

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
	std::cout << "exported scene getted" << std::endl;
	Assimp::Exporter mAiExporter;

	// realizamos concatenacion de path final
	std::string myStrName = myName;
	std::string fullpath = "./modelo3d/custom/" + myStrName + ".obj";

	std::cout << "full path export: " << fullpath << std::endl;

	if (mAiExporter.Export(&myExpScene, "obj", fullpath) != AI_SUCCESS) {
		cerr << mAiExporter.GetErrorString() << endl;
		std::cout << mAiExporter.GetErrorString() << std::endl;
	}

	std::cout << "Exported correct!" << std::endl;

	//delete[] &fullpath;
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
	myIndexes.push_back(1);

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

Model import_export::Hexaedro() {

	vector<SimpleVertex> myVertices;

	SimpleVertex v1;
	glm::vec3 vec1;
	vec1.x = -1.0f;
	vec1.y = -1.0f;
	vec1.z = -1.0f;
	v1.Position = vec1;

	SimpleVertex v2;
	glm::vec3 vec2;
	vec2.x = 1.0f;
	vec2.y = -1.0f;
	vec2.z = -1.0f;
	v2.Position = vec2;

	SimpleVertex v3;
	glm::vec3 vec3;
	vec3.x = 1.0f;
	vec3.y = -1.0f;
	vec3.z = 1.0f;
	v3.Position = vec3;

	SimpleVertex v4;
	glm::vec3 vec4;
	vec4.x = -1.0f;
	vec4.y = -1.0f;
	vec4.z = 1.0f;
	v4.Position = vec4;

	SimpleVertex v5;
	glm::vec3 vec5;
	vec5.x = -1.0f;
	vec5.y = 1.0f;
	vec5.z = -1.0f;
	v5.Position = vec5;

	SimpleVertex v6;
	glm::vec3 vec6;
	vec6.x = 1.0f;
	vec6.y = 1.0f;
	vec6.z = -1.0f;
	v6.Position = vec6;

	SimpleVertex v7;
	glm::vec3 vec7;
	vec7.x = 1.0f;
	vec7.y = 1.0f;
	vec7.z = 1.0f;
	v7.Position = vec7;

	SimpleVertex v8;
	glm::vec3 vec8;
	vec8.x = -1.0f;
	vec8.y = 1.0f;
	vec8.z = 1.0f;
	v8.Position = vec8;

	myVertices.push_back(v1);
	myVertices.push_back(v2);
	myVertices.push_back(v3);
	myVertices.push_back(v4);
	myVertices.push_back(v5);
	myVertices.push_back(v6);
	myVertices.push_back(v7);
	myVertices.push_back(v8);

	// definimos un color
	aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

	// definimos indices de orden de renderizado para un hexaedro
	// definimos indices de orden de renderizado para un hexaedro
	vector<unsigned int> myIndexes;
	myIndexes.push_back(0);
	myIndexes.push_back(1);
	myIndexes.push_back(2);
	myIndexes.push_back(0);
	myIndexes.push_back(2);
	myIndexes.push_back(3);

	myIndexes.push_back(4);
	myIndexes.push_back(5);
	myIndexes.push_back(6);
	myIndexes.push_back(4);
	myIndexes.push_back(6);
	myIndexes.push_back(7);

	myIndexes.push_back(0);
	myIndexes.push_back(4);
	myIndexes.push_back(7);
	myIndexes.push_back(0);
	myIndexes.push_back(7);
	myIndexes.push_back(3);

	myIndexes.push_back(1);
	myIndexes.push_back(5);
	myIndexes.push_back(6);
	myIndexes.push_back(1);
	myIndexes.push_back(6);
	myIndexes.push_back(2);

	myIndexes.push_back(0);
	myIndexes.push_back(1);
	myIndexes.push_back(5);
	myIndexes.push_back(0);
	myIndexes.push_back(5);
	myIndexes.push_back(4);

	myIndexes.push_back(2);
	myIndexes.push_back(3);
	myIndexes.push_back(7);
	myIndexes.push_back(2);
	myIndexes.push_back(7);
	myIndexes.push_back(6);



	Model myModel = Model();
	myModel.pushMesh(myVertices, myColor, myIndexes);

	return myModel;
}

Model import_export::Octaedro() {

	vector<SimpleVertex> myVertices;

	SimpleVertex v1;
	glm::vec3 vec1;
	vec1.x = 0.0f;
	vec1.y = 1.0f;
	vec1.z = 0.0f;
	v1.Position = vec1;

	SimpleVertex v2;
	glm::vec3 vec2;
	vec2.x = 1.0f;
	vec2.y = 0.0f;
	vec2.z = 0.0f;
	v2.Position = vec2;

	SimpleVertex v3;
	glm::vec3 vec3;
	vec3.x = 0.0f;
	vec3.y = 0.0f;
	vec3.z = 1.0f;
	v3.Position = vec3;

	SimpleVertex v4;
	glm::vec3 vec4;
	vec4.x = -1.0f;
	vec4.y = 0.0f;
	vec4.z = 0.0f;
	v4.Position = vec4;

	SimpleVertex v5;
	glm::vec3 vec5;
	vec5.x = 0.0f;
	vec5.y = 0.0f;
	vec5.z = -1.0f;
	v5.Position = vec5;

	SimpleVertex v6;
	glm::vec3 vec6;
	vec6.x = 0.0f;
	vec6.y = -1.0f;
	vec6.z = 0.0f;
	v6.Position = vec6;


	myVertices.push_back(v1);
	myVertices.push_back(v2);
	myVertices.push_back(v3);
	myVertices.push_back(v4);
	myVertices.push_back(v5);
	myVertices.push_back(v6);

	// definimos un color
	aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

	// definimos indices de orden de renderizado para un hexaedro
	vector<unsigned int> myIndexes;
	myIndexes.push_back(0);
	myIndexes.push_back(1);
	myIndexes.push_back(2);

	myIndexes.push_back(0);
	myIndexes.push_back(2);
	myIndexes.push_back(3);

	myIndexes.push_back(0);
	myIndexes.push_back(3);
	myIndexes.push_back(4);

	myIndexes.push_back(0);
	myIndexes.push_back(4);
	myIndexes.push_back(1);

	myIndexes.push_back(5);
	myIndexes.push_back(1);
	myIndexes.push_back(2);

	myIndexes.push_back(5);
	myIndexes.push_back(2);
	myIndexes.push_back(3);

	myIndexes.push_back(5);
	myIndexes.push_back(3);
	myIndexes.push_back(4);

	myIndexes.push_back(5);
	myIndexes.push_back(4);
	myIndexes.push_back(1);

	Model myModel = Model();
	myModel.pushMesh(myVertices, myColor, myIndexes);

	return myModel;
}

Model import_export::Icosaedro() {

	vector<SimpleVertex> myVertices;

	SimpleVertex v1;
	glm::vec3 vec1;
	vec1.x = 0.0f;
	vec1.y = 1.0f;
	vec1.z = 0.0f;
	v1.Position = vec1;

	SimpleVertex v2;
	glm::vec3 vec2;
	vec2.x = 0.0f;
	vec2.y = 0.4472f;
	vec2.z = 0.8944f;
	v2.Position = vec2;

	SimpleVertex v3;
	glm::vec3 vec3;
	vec3.x = 0.8507f;
	vec3.y = 0.4472f;
	vec3.z = 0.2764f;
	v3.Position = vec3;

	SimpleVertex v4;
	glm::vec3 vec4;
	vec4.x = 0.5257f;
	vec4.y = 0.4472f;
	vec4.z = -0.7236f;
	v4.Position = vec4;

	SimpleVertex v5;
	glm::vec3 vec5;
	vec5.x = -0.5257f;
	vec5.y = 0.4472f;
	vec5.z = -0.7236f;
	v5.Position = vec5;

	SimpleVertex v6;
	glm::vec3 vec6;
	vec6.x = -0.8507f;
	vec6.y = 0.4472f;
	vec6.z = 0.2764f;
	v6.Position = vec6;

	SimpleVertex v7;
	glm::vec3 vec7;
	vec7.x = 0.5257f;
	vec7.y = -0.4472f;
	vec7.z = 0.7236f;
	v7.Position = vec7;

	SimpleVertex v8;
	glm::vec3 vec8;
	vec8.x = 0.8507f;
	vec8.y = -0.4472f;
	vec8.z = -0.2764f;
	v8.Position = vec8;

	SimpleVertex v9;
	glm::vec3 vec9;
	vec9.x = 0.0f;
	vec9.y = -0.4472f;
	vec9.z = -0.8944f;
	v9.Position = vec9;

	SimpleVertex v10;
	glm::vec3 vec10;
	vec10.x = -0.8507f;
	vec10.y = -0.4472f;
	vec10.z = -0.2764f;
	v10.Position = vec10;

	SimpleVertex v11;
	glm::vec3 vec11;
	vec11.x = -0.5257f;
	vec11.y = -0.4472f;
	vec11.z = 0.7236f;
	v11.Position = vec11;

	SimpleVertex v12;
	glm::vec3 vec12;
	vec12.x = 0.0f;
	vec12.y = -1.0f;
	vec12.z = 0.0f;
	v12.Position = vec12;



	myVertices.push_back(v1);
	myVertices.push_back(v2);
	myVertices.push_back(v3);
	myVertices.push_back(v4);
	myVertices.push_back(v5);
	myVertices.push_back(v6);
	myVertices.push_back(v7);
	myVertices.push_back(v8);
	myVertices.push_back(v9);
	myVertices.push_back(v10);
	myVertices.push_back(v11);
	myVertices.push_back(v12);

	// definimos un color
	aiColor3D myColor = aiColor3D(0.3f, 0.5f, 0.3f);

	// definimos indices de orden de renderizado para un hexaedro
	// Definimos índices de orden de renderizado para un icosaedro
	vector<unsigned int> myIndexes;
	myIndexes.push_back(0);
	myIndexes.push_back(1);
	myIndexes.push_back(2);

	myIndexes.push_back(0);
	myIndexes.push_back(2);
	myIndexes.push_back(3);

	myIndexes.push_back(0);
	myIndexes.push_back(3);
	myIndexes.push_back(4);

	myIndexes.push_back(0);
	myIndexes.push_back(4);
	myIndexes.push_back(5);

	myIndexes.push_back(0);
	myIndexes.push_back(5);
	myIndexes.push_back(1);

	myIndexes.push_back(1);
	myIndexes.push_back(6);
	myIndexes.push_back(7);

	myIndexes.push_back(1);
	myIndexes.push_back(7);
	myIndexes.push_back(2);

	myIndexes.push_back(2);
	myIndexes.push_back(7);
	myIndexes.push_back(8);

	myIndexes.push_back(2);
	myIndexes.push_back(8);
	myIndexes.push_back(3);

	myIndexes.push_back(3);
	myIndexes.push_back(8);
	myIndexes.push_back(9);

	myIndexes.push_back(3);
	myIndexes.push_back(9);
	myIndexes.push_back(4);

	myIndexes.push_back(4);
	myIndexes.push_back(9);
	myIndexes.push_back(10);

	myIndexes.push_back(4);
	myIndexes.push_back(10);
	myIndexes.push_back(5);

	myIndexes.push_back(5);
	myIndexes.push_back(10);
	myIndexes.push_back(6);

	myIndexes.push_back(5);
	myIndexes.push_back(6);
	myIndexes.push_back(1);

	myIndexes.push_back(6);
	myIndexes.push_back(10);
	myIndexes.push_back(9);

	myIndexes.push_back(7);
	myIndexes.push_back(6);
	myIndexes.push_back(9);

	myIndexes.push_back(8);
	myIndexes.push_back(7);
	myIndexes.push_back(9);

	myIndexes.push_back(7);
	myIndexes.push_back(10);
	myIndexes.push_back(11);

	myIndexes.push_back(8);
	myIndexes.push_back(7);
	myIndexes.push_back(10);


	Model myModel = Model();
	myModel.pushMesh(myVertices, myColor, myIndexes);

	return myModel;

}





