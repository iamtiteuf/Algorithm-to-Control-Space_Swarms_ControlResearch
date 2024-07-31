#ifndef SCRIPTING_AREA_H
#define SCRIPTING_AREA_H
#include <Python.h>

#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
#include "gui/imgui_stdlib.h"
#include "gui/imgui_internal.h"
#include "glm/glm.hpp"
#include "gui/TextEditor.h"
#include "Glew_Initialization.h"



class Script_Editor
{
public:
    Script_Editor()
    {
        Py_Initialize();
    }
    ~Script_Editor()
    {
        Py_Finalize();
    }
    void OpenWindow();
    void ForceEditor();
    void RunScript(double n_c);
    bool Text_init = false;
    glm::vec3 Force;
    TextEditor editor;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();


private:
    std::string Vector_Class = R"(

import numpy as np
import control


A = np.array([[0, 0, 0, 1, 0, 0],
  [0, 0, 0, 0, 1, 0],
  [0, 0, 0, 0, 0, 1],
  [3*n*n, 0, 0, 0, 2*n, 0],
  [0, 0, 0, -2*n, 0, 0],
  [0, 0, -n*n, 0, 0, 0]])

B = np.array([[0, 0, 0],
  [0, 0, 0],
  [0, 0, 0],
  [1/mass, 0, 0],
  [0, 1/mass, 0],
  [0, 0, 1/mass]])

C = np.eye(6)
D = np.zeros((6, 3))

Q = np.eye(6)
R = np.eye(3)*1/10
sys = control.ss(A, B, C, D)

K, S, E = control.lqr(A, B, Q, R)

K_list = K.tolist()



)";


    std::string Force_str = Vector_Class;



  //  K, S, E = lqr(A, B, Q, R)


};
#endif

