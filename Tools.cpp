#include "Tools.h"
#include <imgui/imgui.h>

#include "main.h"
#include "Renderizador.h"
#include "impExp.h"
#include <iostream>

#include <ImGuiFileDialog/ImGuiFileDialog.h>

ImGuiFileDialog fileDialog;

//static bool show_window_model = true;

//// Helper to wire demo markers located in code to an interactive browser
//typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
//extern ImGuiDemoMarkerCallback      GImGuiDemoMarkerCallback;
//extern void* GImGuiDemoMarkerCallbackUserData;
//ImGuiDemoMarkerCallback             GImGuiDemoMarkerCallback = NULL;
//void* GImGuiDemoMarkerCallbackUserData = NULL;
//#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)
namespace GuiTools {
    bool show_window_model = false;
    bool show_window_color = false;
    bool show_window_effects = false;
    bool show_window_shape = false;
    bool show_window_objinfo = false;
    bool show_window_objinfo_colorSelect = false;
    bool show_window_objexp = false;
    bool show_window_objimport = false;
    bool show_window_AboutUs = false;

    int selectedModelIndex = -1;

    void GuiTools::BarraHerramientas()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ShowMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::BeginMenu("Import")) {
                    if (ImGui::MenuItem("Tetraedro")) {
                        Model tmpModel = import_export::Tetraedro();
                        import_export::Importation(tmpModel);
                    }
                    
                    if (ImGui::MenuItem("Hexaedro")) {
                        Model tmpModel = import_export::Hexaedro();
                        import_export::Importation(tmpModel);
                    }
                    
                    if (ImGui::MenuItem("Octaedro")) {
                        Model tmpModel = import_export::Octaedro();
                        import_export::Importation(tmpModel);
                    }
                    
                    if (ImGui::MenuItem("Icosaedro")) {
                        Model tmpModel = import_export::Icosaedro();
                        import_export::Importation(tmpModel);
                    }
                    if (ImGui::MenuItem("Other...")) {
                        if (show_window_objimport) {
                            show_window_objimport = false;
                        } else {
                            show_window_objimport = true;
                        }
                        ShowObjimportWindow(&show_window_objimport);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Export")) {
                    if (show_window_objexp) {
                        show_window_objexp = false;
                    } else {
                        show_window_objexp = true;
                    }
                    ShowObjexpWindow(&show_window_objexp);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("Model")) {
                    if (show_window_model) {
                        show_window_model = false;
                    } else {
                        show_window_model = true;
                    }
                }
                if (ImGui::MenuItem("Background Color")) {
                    if (show_window_color) {
                        show_window_color = false;
                    } else {
                        show_window_color = true;
                    }
                }
                if (ImGui::MenuItem("Effects")) {
                    if (show_window_effects) {
                        show_window_effects = false;
                    } else {
                        show_window_effects = true;
                    }
                }
                if (ImGui::MenuItem("Shape")) {
                    if (show_window_shape) {
                        show_window_shape = false;
                    } else {
                        show_window_shape = true;
                    }
                }
                if (ImGui::MenuItem("Object Info")) {
                    if (show_window_objinfo) {
                        show_window_objinfo = false;
                    } else {
                        show_window_objinfo = true;
                    }
                    ShowObjinfoWindow(&show_window_objinfo, &show_window_objinfo_colorSelect);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About")) {
                if (ImGui::MenuItem("About Us")) 
                {
                    if (show_window_AboutUs)
                    {
                        show_window_AboutUs = false;
                    } else {
                        show_window_AboutUs = true;
                    }
                    ShowAboutUsWindow(&show_window_AboutUs);
                }
                if (ImGui::MenuItem("About Project")) {}
                if (ImGui::MenuItem("Help")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    static void GuiTools::ShowMenuFile()
    {
        if (ImGui::MenuItem("New")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::MenuItem("fish_hat.c");
            ImGui::MenuItem("fish_hat.inl");
            ImGui::MenuItem("fish_hat.h");
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As..")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {}
    }

    void GuiTools::ShowModelWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("Model");
            ImGui::Text("Esta es la ventana de Herramientas de Modelado");
            ImGui::End();
        }
    }
    void GuiTools::ShowColorWindow(bool* p_open) {
        if (*p_open)
        {
            ImGui::Begin("Background Color");
            ImGui::Text("Esta es la ventana de Herramientas de Color");

            static ImVec4 color = prog_state::globalBackgroudColor; // Inicializar con el valor global
            // Utiliza la función ImGui::ColorEdit4 correctamente pasando la dirección de 'color'
            ImGui::ColorEdit4("Select background color", (float*)&color);
            prog_state::globalBackgroudColor = color;

            ImGui::End();
        }
    }
    void GuiTools::ShowEffectsWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("Effects");
            ImGui::Text("Esta es la ventana de Herramientas de Modelado");
            ImGui::End();
        }
    }
    void GuiTools::ShowShapeWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("Shape");
            ImGui::Text("Esta es la ventana de Herramientas de Modelado");
            ImGui::End();
        }
    }

    void GuiTools::ShowAboutUsWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("About Us");
            ImGui::Text("Cradores:");
            ImGui::Text("Sebas Guaman");
            ImGui::Text("Johanna Arias");
            ImGui::Text("Kleber Saavedra");
            ImGui::Text("Quito, 25 junio 2023");
            ImGui::End();
        }
    }

    void GuiTools::ShowObjinfoWindow(bool* p_open, bool* c_open) {
        if (*p_open) {
            ImGui::Begin("Object Info", p_open);
            for (int i = 0; i < prog_state::stateModels.size(); i++) {
                Model &myModel = prog_state::stateModels[i];
                aiColor3D tmpColor = myModel.myColor();
                ImVec4 textColor(tmpColor.r, tmpColor.g, tmpColor.b, 1.0f);
                //std::cout << myModel.meshes[0].simpleVertices.size() << std::endl;
                Mesh tmpMesh = myModel.meshes[0];
                int vertSize = tmpMesh.simpleVertices.size();

                std::string tmpTitle = "Model " + std::to_string(i) + "; Num_Vertex " + std::to_string(vertSize);
                const char* titleChar = tmpTitle.c_str();

                //std::cout << "formated title: " << tmpTitle << std::endl;
                ImGuiStyle& style = ImGui::GetStyle();
                style.Colors[ImGuiCol_Text] = textColor;

                ImGui::PushID(i);

                if (ImGui::Button("Color")) {
                    show_window_objinfo_colorSelect = !show_window_objinfo_colorSelect;
                    selectedModelIndex = i;
                }

                ImGui::PopID();
                
                ImGui::SameLine();
                if (ImGui::CollapsingHeader(titleChar)) {
                    for (int j = 0; j < vertSize; j++) {
                        float x_pos = tmpMesh.simpleVertices[j].Position.x;
                        float y_pos = tmpMesh.simpleVertices[j].Position.y;
                        float z_pos = tmpMesh.simpleVertices[j].Position.z;
                        std::string tmpVertex = "Vertex " + std::to_string(j) + " x:" + std::to_string(x_pos) + " y:" + std::to_string(y_pos) + " z:" + std::to_string(z_pos);
                        const char* vertexChar = tmpVertex.c_str();
                        ImGui::BulletText(vertexChar);
                    }
                }
                
                style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

                /*ImGui::End();*/

                //CollapsingHeader
                //ImGui::TextColored(textColor, "Model %d; Num_Vertex %d",i,vertSize);
                /*ImGui::TextColored(textColor, "Model %d: x=%.2f\ty=%.2f\tz=%.2f", i,
                x_pos,
                y_pos,
                z_pos);*/
            }

            if (*c_open && selectedModelIndex >= 0 && selectedModelIndex < prog_state::stateModels.size()) {
                Model& selectedModel = prog_state::stateModels[selectedModelIndex];
                aiColor3D someColor = selectedModel.myColor();

                ImGui::Begin("Select color", c_open);
                if (ImGui::ColorEdit3("Selecciona un color", (float*)&someColor)) {
                    selectedModel.myColor(someColor);
                }
                ImGui::End();
            }
            ImGui::End();
        }
    }

    void GuiTools::ShowObjexpWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("Object Export", p_open);
            for (int i = 0; i < prog_state::stateModels.size(); i++) {
                Model& myModel = prog_state::stateModels[i];
                aiColor3D tmpColor = myModel.myColor();
                ImVec4 textColor(tmpColor.r, tmpColor.g, tmpColor.b, 1.0f);
                //std::cout << myModel.meshes[0].simpleVertices.size() << std::endl;
                Mesh tmpMesh = myModel.meshes[0];
                int vertSize = tmpMesh.simpleVertices.size();

                std::string tmpTitle = "Model " + std::to_string(i) + "; Num_Vertex " + std::to_string(vertSize);
                const char* titleChar = tmpTitle.c_str();

                //std::cout << "formated title: " << tmpTitle << std::endl;
                ImGuiStyle& style = ImGui::GetStyle();
                style.Colors[ImGuiCol_Text] = textColor;

                ImGui::PushID(i);

                if (ImGui::Button("Export")) {
                    selectedModelIndex = i;
                    Model toExport = prog_state::stateModels[i];
                    std::cout << "exporting " << selectedModelIndex << std::endl;
                    std::string toExpName = "Model" + std::to_string(i);
                    import_export::Exportation(toExport, &toExpName[0]);
                }

                ImGui::PopID();

                ImGui::SameLine();
                ImGui::Text(titleChar);
                /*if (ImGui::CollapsingHeader(titleChar)) {
                    for (int j = 0; j < vertSize; j++) {
                        float x_pos = tmpMesh.simpleVertices[j].Position.x;
                        float y_pos = tmpMesh.simpleVertices[j].Position.y;
                        float z_pos = tmpMesh.simpleVertices[j].Position.z;
                        std::string tmpVertex = "Vertex " + std::to_string(j) + " x:" + std::to_string(x_pos) + " y:" + std::to_string(y_pos) + " z:" + std::to_string(z_pos);
                        const char* vertexChar = tmpVertex.c_str();
                        ImGui::BulletText(vertexChar);
                    }
                }*/

                style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

                /*ImGui::End();*/

                //CollapsingHeader
                //ImGui::TextColored(textColor, "Model %d; Num_Vertex %d",i,vertSize);
                /*ImGui::TextColored(textColor, "Model %d: x=%.2f\ty=%.2f\tz=%.2f", i,
                x_pos,
                y_pos,
                z_pos);*/
            }

            //if (*c_open && selectedModelIndex >= 0 && selectedModelIndex < prog_state::stateModels.size()) {
            //    Model& selectedModel = prog_state::stateModels[selectedModelIndex];
            //    aiColor3D someColor = selectedModel.myColor();

            //    ImGui::Begin("Select color", c_open);
            //    if (ImGui::ColorEdit3("Selecciona un color", (float*)&someColor)) {
            //        selectedModel.myColor(someColor);
            //    }
            //    ImGui::End();
            //}
            ImGui::End();
        }
    }

    void GuiTools::ShowObjimportWindow(bool* p_open) {
        if (*p_open) {
            //ImGui::Begin("Object Import", p_open);
            fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
            if (fileDialog.Display("ChooseFileDlgKey")) {
                if (fileDialog.IsOk()) {
                    std::string filePathName = fileDialog.GetFilePathName();
                    std::string filePath = fileDialog.GetCurrentPath();
                    std::cout << "filepathName: " << filePathName << std::endl;
                    std::cout << "filepath: " << filePath << std::endl;
                }
            }
            //ImGui::End();
        }
    }
}
