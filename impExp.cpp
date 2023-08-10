#include "impExp.h"

void import_export::Importation(const char* modelObjPath) {
	// importacion del modelo desde un obj que tien un mtl
	Model tmpModel(modelObjPath);
	import_export::model = tmpModel;

	//vector<SimpleVertex> myVertices = import_export::model.meshes.size();
	//vector<Texture> textures;
	//aiColor3D myColor = aiColor3D();
}