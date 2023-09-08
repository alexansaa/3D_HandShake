#ifndef IMPORT_EXPORT
#include "model.h"
namespace import_export {
	extern Model model;
	extern Model tmpModel;
	void Importation(const char* modelObjPath, bool processCustom);
	void Importation(Model myModel);
	void ImportationAcepted();
	void ImportationCanceled();
	void ImportationTraslation();
	void Exportation(Model myModel, char* myName);
	Model Tetraedro();
}
#endif
