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
	strcpy(fullpath, root);
	strcpy(fullpath, myName);
	strcpy(fullpath, ext);

	if (mAiExporter.Export(&myExpScene, "obj", fullpath) != AI_SUCCESS) {
		cerr << mAiExporter.GetErrorString() << endl;
		std::cout << mAiExporter.GetErrorString() << std::endl;
	}

	delete[] fullpath;
}