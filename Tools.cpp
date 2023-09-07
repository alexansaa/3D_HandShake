#include "Tools.h"
#include <imgui/imgui.h>

#include "main.h"
#include "Renderizador.h"
#include <iostream>

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
    bool show_window_AboutUs = false;

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
                if (ImGui::MenuItem("Import")) {}
                if (ImGui::MenuItem("Export")) {}
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
                if (ImGui::MenuItem("Color")) {
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
                    ShowObjinfoWindow(&show_window_objinfo);
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
            ImGui::Begin("Color");
            ImGui::Text("Esta es la ventana de Herramientas de Color");

            static ImVec4 color = prog_state::globalBackgroudColor; // Inicializar con el valor global
            // Utiliza la función ImGui::ColorEdit4 correctamente pasando la dirección de 'color'
            ImGui::ColorEdit4("Selecciona un color", (float*)&color);
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

    void GuiTools::ShowObjinfoWindow(bool* p_open) {
        if (*p_open) {
            ImGui::Begin("Object Info");
            for (int i = 0; i < prog_state::stateModels.size(); i++) {
                Model myModel = prog_state::stateModels[i];
                aiColor3D tmpColor = myModel.myColor();
                ImVec4 textColor(tmpColor.r, tmpColor.g, tmpColor.b, 1.0f);
                //std::cout << myModel.meshes[0].simpleVertices.size() << std::endl;
                Mesh tmpMesh = myModel.meshes[0];
                int vertSize = tmpMesh.vertices.size();
                ImGui::TextColored(textColor, "Model %d; Num_Vertex %d",i,vertSize);
                /*ImGui::TextColored(textColor, "Model %d: x=%.2f\ty=%.2f\tz=%.2f", i,
                x_pos,
                y_pos,
                z_pos);*/
            }
            ImGui::End();
        }
    }
}
