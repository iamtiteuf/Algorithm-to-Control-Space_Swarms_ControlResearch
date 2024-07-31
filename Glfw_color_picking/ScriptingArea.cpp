#include "ScriptingArea.h"

#include <iostream>

void Script_Editor::OpenWindow()
{
	ImGui::BeginChild("Force Script");
	
	//ImGui::SetWindowPos(ImVec2(0, 24), 0);

	ForceEditor();

	ImGui::EndChild();
}
void Script_Editor::ForceEditor()
{


	ImGui::SetWindowFontScale(0.9f); // Increase font size by 50%

	if (Text_init == false)
	{
		editor.SetText(Force_str);
		Text_init = true;
	}


	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
	//ImGui::InputTextMultiline("script", &Force_str, ImVec2(0, 0), 0, NULL, NULL);
	editor.SetReadOnly(false);
	editor.SetPalette(TextEditor::GetDarkPalette());
	editor.SetReadOnly(false);
	TextEditor::Palette palette;

	/*palette[(unsigned)TextEditor::PaletteIndex::Keyword] = ImColor(255, 255, 255);
	editor.SetPalette(palette);*/


	lang.mKeywords.insert("Force");
	lang.mKeywords.insert("class");
	lang.mKeywords.insert("def");
	lang.mKeywords.insert("elif");
	lang.mKeywords.insert("vec3");

	editor.SetLanguageDefinition(lang);

	Force_str = editor.GetText();




	editor.Render("MyTextEditor");




	//ImGui::SetWindowFontScale(1.0f); // Increase font size by 50%

	// Initialize the Python interpreter (should be done only once)
	

}
void Script_Editor::RunScript(double n_c)
{
	PyObject* module = PyImport_AddModule("__main__");

	PyObject* n_py = PyFloat_FromDouble(n_c);
	PyObject* mass_py = PyFloat_FromDouble(c_mass);
	// Set the value of the Python variable
	PyObject_SetAttrString(module, "n", n_py);
	PyObject_SetAttrString(module, "mass", mass_py);
	// Get the value of the variable
	const char* Force_c_str = Force_str.c_str();

	PyRun_SimpleString(Force_c_str);

	PyObject* p_var = PyObject_GetAttrString(module, "K_list");
	if (p_var == NULL)
	{
		PyErr_Print();
		std::cout << "Error occurred while retrieving variable 'K_list'" << std::endl;
	}
	else {
		Py_INCREF(p_var);
	}

	// Convert the Python object to a C++ Eigen::MatrixXd
	if (PyList_Check(p_var)) {
		int rows = PyList_Size(p_var);
		int cols = PyList_Size(PyList_GetItem(p_var, 0));
		Eigen::MatrixXd K = Eigen::MatrixXd::Zero(rows, cols);
		for (int i = 0; i < rows; i++) {
			PyObject* row = PyList_GetItem(p_var, i);
			for (int j = 0; j < cols; j++) {
				PyObject* item = PyList_GetItem(row, j);
				if (PyFloat_Check(item)) {
					K(i, j) = PyFloat_AsDouble(item);
				}
			}
		}
	    Gain = K;
	}
	else
	{
		std::cout << "'K_list' is not a list of lists" << std::endl;
	}


	 //Decrement the reference count of the Python object
	if (n_py != NULL) {
		Py_DECREF(n_py);
	}
	if (mass_py != NULL) {
		Py_DECREF(mass_py);
	}
	if (p_var != NULL) {
		Py_DECREF(p_var);
	}
}