#include "impExp.h"

void import_export::Importation(const char* modelObjPath, bool processCustom) {
	// importacion del modelo desde un obj que tien un mtl
	Model tmpModel(modelObjPath, processCustom);
	import_export::model = tmpModel;
}