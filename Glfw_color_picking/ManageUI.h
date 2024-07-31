#ifndef MANAGEUI_CLASS_H
#define MANAGEUI_CLASS_H
#include	"implot.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "imgui_internal.h"
#include "ControlScript.h"
#include "ScriptingArea.h"
class Manage_Widgets
{
public:
	void manage(ControlScript &Control, bool &leftclicked,int &Cube_ID,int &Cube2_ID, bool &start_Simulation,Script_Editor &script,All_Potential_Type& potential_type)
	{
		Pop_up_OVERLAY(Control);
		
		/*bool* opentri = new bool(true);
		ImGui::ShowDemoWindow(opentri);*/
		Tool_Bar(Control, Cube_ID, start_Simulation, Cube2_ID, leftclicked);
		task_bar(start_Simulation,Control, Cube_ID, Cube2_ID,leftclicked);
		
		ImVec2 win_pos;
		win_pos = Get_Win_Pos(0);
		if (*Settings_window == true)
		{
			ImGui::Begin("Settings", Settings_window);
			
			

			ImGui::SetWindowPos(win_pos);
			Spawn_Settings(Control,leftclicked,Cube_ID,Cube2_ID,start_Simulation, potential_type);
			
			ObjectTransforms(Control, Cube_ID);
			ImGui::Checkbox("Show Script", &ShowScript);
			ImGui::Checkbox("Apply CLW", &ApplyCLW);
			if (ShowScript)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("scripts child", ImVec2(0, 500), true, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Scripts.."))
					{
						ImGui::MenuItem("World Script");
						ImGui::MenuItem("Object Script");
						ImGui::EndMenu();
					}
					
				}
				
				ImGui::EndMenuBar();
				ImGui::SeparatorText("In Python");
				script.OpenWindow();


				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
			

			if (ImGui::Combo("Line Type", reinterpret_cast<int*>(&Control.linetype), "Original\0Dijkstra\0Kruskals\0")) {
				// The selected item is stored in the variable linetype
			}
			ImGui::Checkbox("Show Connecting Line", &Connecting_Line);

			if (Control.All_Meshs.size() > 0)
			{
				earth(Control.All_Meshs[0]);
	
			}
			if (ImGui::CollapsingHeader("Check Control"))
			{
				ImGui::Checkbox("Apply LQR", &ApplyLqr);
				
				if (ImGui::CollapsingHeader("Potential Based Control"))
				{

					if (ImGui::Combo("Potential Type", (int*)&potential_type, "Complex\0Harmonic_Potential\0Lennard_Jones_Potential\0Mei_Potential\0Virtual_Potential\0")) {
						// The selected item is stored in the variable line
					}
					ImGui::Checkbox("Apply Potential To center", &ApplyPotential_To_One);
					ImGui::Checkbox("Add repulsize Potential", &add_repulsive_potential);
					switch (potential_type)
					{
					case 0:
						ImGui::Checkbox("Apply Control ", &ApplyPotential);
						ImGui::InputFloat("Ca", &Ca, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Cr", &Cr, 0.02, 0.01);
						ImGui::InputFloat("la", &la, 0.02, 0.01);
						ImGui::InputFloat("lr", &lr, 0.02, 0.01);
						ImGui::InputFloat("fr", &fr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Alpha", &alpha, 0.02, 0.01, "%.6f");
						break;
					case 1:
						ImGui::Checkbox("Apply Control ", &ApplyPotential);
						ImGui::InputFloat("K", &Ca, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Kr", &Cr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("friction scale",&lr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("velocity max", &la, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("fr", &fr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Alpha", &alpha, 0.02, 0.01, "%.6f");
						break;
					case 2:
						ImGui::Checkbox("Apply Control ", &ApplyPotential);
						ImGui::InputFloat("E", &E, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("fr", &fr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Alpha", &alpha, 0.02, 0.01, "%.6f");
						break;
					case 3:
						ImGui::Checkbox("Apply Control ", &ApplyPotential);
						ImGui::InputFloat("E", &E, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("n", &Ca, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("m", &Cr, 0.02, 0.01);
						ImGui::InputFloat("fr", &fr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Alpha", &alpha, 0.02, 0.01, "%.6f");
						break;
					case 4:
						ImGui::Checkbox("Apply Control ", &ApplyPotential);
						ImGui::InputFloat("K", &Ca, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Kr", &Cr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("fr", &fr, 0.0002, 0.0001, "%.6f");
						ImGui::InputFloat("Alpha", &alpha, 0.02, 0.01, "%.6f");
						break;
					}
					
				}
			}
			
			

			

			ImGui::End();
		}

		if (*Analysis_window == true)
		{
			Analytics(Control, Cube_ID);
		}
	}

	void task_bar(bool& start_Simulation, ControlScript& Control, int Cube_ID, int& Cube2_ID, bool& leftclicked)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
			    if (ImGui::MenuItem("New"))
				{

				}
				if (ImGui::MenuItem("Open"))
				{
				}
				if (ImGui::MenuItem("Save"))
				{
				}
				if (ImGui::MenuItem("Save as"))
				{
				}
				if (ImGui::BeginMenu("Options"))
				{
					ImGui::Checkbox("Mouse Overlay", &can_read_Overlay);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("system type"))
			{
				if (ImGui::BeginMenu("Control sys"))
				{
					const char* name;

					if (*Settings_window == false)
					{
						name = "Open CLW sys";
					}
					else
					{
						name = "Close CLW sys";
					}
					if (ImGui::MenuItem(name))
					{
						if (name == "Open CLW sys")
						{
							*Settings_window = true;
						}
						else
						{
							*Settings_window = false;
						}
					}
					if (ImGui::MenuItem("Orbit"))
					{ 
						
						Control.eliptic_orbit->gen_line();
						Control.eliptic_orbit->transform.position = Control.eliptic_orbit->line_points[0];
						Control.eliptic_orbit->move_on_elipse = !Control.eliptic_orbit->move_on_elipse;
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Analysis"))
			{
				const char* name;
				if (*Analysis_window == false)
				{
					name = "Open Analysis";
				}
				else
				{
					name = "Close Analysis";
				}
				if (ImGui::MenuItem(name))
				{
					if (name == "Open Analysis")
					{
						*Analysis_window = true;
					}
					else
					{
						*Analysis_window = false;
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Simulation"))
			{
				if (start_Simulation)
				{
					if (ImGui::MenuItem("Simulating"))
					{
						start_Simulation = false;
					}
				}
				else 
				{ 
					if (ImGui::MenuItem("Simulate"))
					{
						start_Simulation = true;
					}
				}
				
				if(ImGui::BeginMenu("Options"))
				{
					ImGui::Checkbox("Simulate", &start_Simulation);
					ImGui::InputFloat("gravitaional Constant", &G_const);
					ImGui::InputFloat("Time Step", &dt);
					ImGui::Checkbox("Rotate to velocity", &Rotate_to_velocity);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				if(ImGui::BeginMenu("Add Objects"))
				{
					if (ImGui::Button("add satelite"))
					{
						Control.Add_Body();
					}
					
					if (ImGui::InputInt("number", &SwarmSize));
					ImGui::Checkbox("UseRandomValueFromFile", &UseRandomValueFromFile);
					if (ImGui::Button("Add in satelites"))
					{
						Control.Int_ControlScript();
					}
					if (ImGui::Button("Bouding Box"))
					{
						Control.Add_Bounding_Box();
					}
					if (Control.All_Meshs.size() > 0)
					{
						if (ImGui::Button("Reset"))
						{
							for (auto obj : Control.All_Meshs)
							{
								if (Control.All_Meshs[0] != obj)
								{
									obj->ClearMesh();
								}

							}
							Control.All_Meshs.clear();
							start_Simulation = false;
							leftclicked = false;
							Cube_ID = 0;
							Cube2_ID = 0;
							Control.nextColorID = glm::vec4(0, 0, 0, 0);
							Control.ID = glm::vec4(0, 0, 0, 0);
							Control.t_data.clear();
							Control.x_data.clear();
							Control.y_data.clear();
							Control.z_data.clear();
						}

					}
					
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

	}

	void Analytics(ControlScript& Control, int &Cube_ID)
	{
		ImVec2 win_pos;
		win_pos = Get_Win_Pos(1);
		if (*transparency) window_flag |= ImGuiWindowFlags_NoBackground;
		else window_flag = 0;

		ImGui::Begin("Analysis", Analysis_window, window_flag);
		ImGui::Checkbox("transparency", transparency);
		ImGui::SetWindowPos(win_pos);

		if (Control.All_Meshs.size() > 1)
		{
			if (Control.NoGroups)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red color
				ImGui::Text("No Groups Found");;
				ImGui::PopStyleColor();
			}
			ImGui::InputInt("Current ID", &Cube_ID);
			ImGui::InputInt("Group Number", &Control.All_Meshs[Cube_ID]->GroupID);
			ImGui::InputFloat("Current Drift", &Control.All_Meshs[Cube_ID]->transform.drift);
			if (ImGui::Checkbox("Trace Motion", &Control.All_Meshs[Cube_ID]->traceMotion))
			{
				if (Control.All_Meshs[Cube_ID]->traceMotion)
				{
					if (Control.All_Meshs[Cube_ID]->TraceInt == -1)
					{
						lineT* line = new lineT;
						Control.All_TraceLines.push_back(line);
						Control.All_Meshs[Cube_ID]->TraceInt = Control.All_TraceLines.size() - 1;
					}
				}
				
			}
			if (Control.All_Meshs[Cube_ID]->traceMotion)
			{
				Control.All_TraceLines[Control.All_Meshs[Cube_ID]->TraceInt]->UpdateLine(Control.All_Meshs[Cube_ID]->transform.position);
				ImGui::Checkbox("Hide Trace", &Control.All_TraceLines[Control.All_Meshs[Cube_ID]->TraceInt]->ShowLine);
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("checking bounds", ImVec2(0, 100), true, ImGuiWindowFlags_MenuBar);
		
		float celocity = glm::length(Control.All_Meshs[Cube_ID]->transform.velocity);
		
		ImGui::InputFloat("Velocity amount", &celocity);

		ImGui::Text("Current space bound :  %0.2f", cur_space_bound_check);
		ImGui::Text("Max space bound     :  %0.2f", max_space_bound_check);
		ImGui::Text("Min space bound     :  %0.2f", min_space_bound_check);

		ImGui::InputFloat("Number of Collisions", &collide_number);
		ImGui::EndChild();
		ImGui::PopStyleVar();

		  
		
		const char* name;
		switch (Cube_ID)
		{
		case 0:
			name = "..Planet..";
			break;
		case 1:
			name = "..Target..";
			break;
		default:
			name = "..Chaser..";
			break;
		}
		if(ImGui::CollapsingHeader("Adjacency Matrix"))
		{
			if (ImGui::BeginTable("Adjacency Matrix", Control.adjacency_mat.cols() + 1, ImGuiTableFlags_Borders)) {
				// Table headers with an extra column for row headers
				ImGui::TableSetupColumn(""); // Empty top-left cell
				for (int col = 0; col < Control.adjacency_mat.cols(); ++col) {
					ImGui::TableSetupColumn(("s" + std::to_string(col + 1)).c_str());
				}
				ImGui::TableHeadersRow();

				// Fill table with matrix data and row headers
				for (int row = 0; row < Control.adjacency_mat.rows(); ++row) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); // First column for row headers
					ImGui::Text(("s" + std::to_string(row + 1)).c_str());
					for (int col = 0; col < Control.adjacency_mat.cols(); ++col) {
						ImGui::TableSetColumnIndex(col + 1); // Offset by 1 for data columns
						if (Control.adjacency_mat(row, col)==0)
						{
							ImGui::Text("%d", static_cast<int>(Control.adjacency_mat(row, col)));
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
							ImGui::Text("%d", static_cast<int>(Control.adjacency_mat(row, col)));
							ImGui::PopStyleColor();
						}
					}
				}

				ImGui::EndTable();
			}
		}
		if (ImGui::CollapsingHeader("LQR Gain : "))
		{
			if (ImGui::BeginChild("Group Table",ImVec2(0,120),ImGuiWindowFlags_MenuBar))
			{
				// Check if the matrix is initialized
				if (!Gain.rows() || !Gain.cols()) {
					ImGui::Text("Matrix not initialized");
					ImGui::End();
					return;
				}

				// Create a new table
				if (ImGui::BeginTable("Gain Matrix ", Gain.cols(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) // +1 for row numbers
				{
					// Draw new row at the top

					for (int j = 0; j < Gain.cols(); ++j)
					{
						/*ImGui::TableNextColumn();
						ImGui::Text("%d", j );*/
					}

					// Draw existing rows
					for (int i = 0; i < Gain.rows(); ++i)
					{
						/*ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("%d", i);*/

						// Draw row cells
						for (int j = 0; j < Gain.cols(); ++j)
						{
							// Check if the indices are within the valid range
							if (i < 0 || i >= Gain.rows() || j < 0 || j >= Gain.cols()) {
								ImGui::TableNextColumn();
								ImGui::Text("Out of bounds");
							}
							else {
								ImGui::TableNextColumn();
								ImGui::Text("%.5e", Gain(i, j)); // Use %.2f to display as float with 2 decimal places
							}
						}
					}
					ImGui::EndTable();
				}
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("Distance Graph"))
		{
			ImGui::BeginChild("distance Graphs", ImVec2(0, 1000), true, ImGuiWindowFlags_MenuBar);
			ImGui::SeparatorText(name);
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);


			if (ImPlot::BeginPlot("r Plot", ImVec2(-1, 700))) {

				ImPlot::PlotLine("|r| vs t :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.position.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			if (ImPlot::BeginPlot("r bar Plot", ImVec2(-1, 700))) {

				ImPlot::PlotLine("sum|ri - rj| vs t Plot :", All_vector_time.data(), All_Vector_Sum.data(), All_vector_time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);


			if (ImPlot::BeginPlot("fuel Plot", ImVec2(-1, 700))) {

				ImPlot::PlotLine("fuel_c vs t :", Fuel_Time.data(), Fuel_Consumption.data(), Fuel_Time.size());


				ImPlot::EndPlot();
			}
			ImGui::EndChild();
		}
		if(ImGui::CollapsingHeader( " Position Graph"))
		{
			ImGui::BeginChild("Position Graphs", ImVec2(0, 1000), true, ImGuiWindowFlags_MenuBar);
			ImGui::SeparatorText(name);
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			
			if (ImPlot::BeginPlot("x Plot")) {

				ImPlot::PlotLine("x Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.position_x.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			if (ImPlot::BeginPlot("y Plot")) {


				ImPlot::PlotLine("y Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.position_y.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			if (ImPlot::BeginPlot("z Plot")) {


				ImPlot::PlotLine("z Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.position_z.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());

				ImPlot::EndPlot();
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("velocity Graph"))
		{
			ImGui::BeginChild("velocity Graphs", ImVec2(0, 1000), true, ImGuiWindowFlags_MenuBar);
			ImGui::SeparatorText(name);

			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			Control.All_Meshs[Cube_ID]->my_plot.velocit.clear();
			float val = 0;
			for (int i = 0; i < Control.All_Meshs[Cube_ID]->my_plot.velocity_x.size(); i++)
			{
				val = glm::length(glm::vec3(Control.All_Meshs[Cube_ID]->my_plot.velocity_x[i], Control.All_Meshs[Cube_ID]->my_plot.velocity_y[i], Control.All_Meshs[Cube_ID]->my_plot.velocity_z[i]));
				Control.All_Meshs[Cube_ID]->my_plot.velocit.push_back(val);
			}
			if (ImPlot::BeginPlot("max Plot")) {

				ImPlot::PlotLine("max Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.velocit.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);


			if (ImPlot::BeginPlot("x Plot")) {

				ImPlot::PlotLine("x Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.velocity_x.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			if (ImPlot::BeginPlot("y Plot")) {


				ImPlot::PlotLine("y Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.velocity_y.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
			if (ImPlot::BeginPlot("z Plot")) {


				ImPlot::PlotLine("z Plot :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.velocity_z.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());

				ImPlot::EndPlot();
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("intergral norm"))
		{
			ImGui::BeginChild("intergral Graphs", ImVec2(0, 1000), true, ImGuiWindowFlags_MenuBar);
			ImGui::SeparatorText(name);
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);


			if (ImPlot::BeginPlot("n plot")) {

				ImPlot::PlotLine("n vs t :", Control.All_Meshs[Cube_ID]->my_plot.time.data(), Control.All_Meshs[Cube_ID]->my_plot.integral_norm.data(), Control.All_Meshs[Cube_ID]->my_plot.time.size());


				ImPlot::EndPlot();
			}
			
			ImGui::EndChild();
		}
		ImGui::End();
	}

	ImVec2 Get_Win_Pos(int index)
	{
		switch (index)
		{
		case 0:
			glfwGetWindowSize(Main_Window, &wid, &hei);
			return ImVec2(0, 23);
			break;
		case 1:
			glfwGetWindowSize(Main_Window, &wid, &hei);
			return ImVec2(wid/1.7, 23);
			break;
		}
	}
	~Manage_Widgets()
	{
		delete Settings_window;
		delete  Analysis_window;
		delete transparency;
	}

	void earth(mesh * mesh)
	{
		if(ImGui::CollapsingHeader("Center Body"))
		{
			float raduis = mesh->transform.raduis;
			if (ImGui::InputFloat("Raduis", &raduis))
			{
				mesh->transform.raduis = raduis;
				mesh->transform.scale = raduis * mesh->transform.scale;
			}
			ImGui::Checkbox("Line ref visibility", &mesh->show_equator_line);
		}
		
	}
	glm::vec2 stereographic_projection(glm::vec3 point)
	{
		const float R = 10.0f; // Radius of the sphere
		float y = point.y;

		if (std::abs(y - R) < 0.001f) {
			// Handle the case where the point is very close to the north pole
			// This could involve returning a point at infinity or handling differently based on requirements
			return glm::vec2(0.0f); // Example handling
		}

		return glm::vec2(R * point.x / (R - y), R * point.z / (R - y));
	}

	void align_points_with_fixed_distance(std::vector<mesh*>& projections, glm::vec2 line_dir, float distance) {
		line_dir = glm::normalize(line_dir);
		glm::vec2 current_position = glm::vec2(-40.0f, 0.0f);

		for (size_t i = 0; i < projections.size(); ++i) {
			projections[i]->transform.position = glm::vec3(current_position.x, 0.01f, current_position.y);
			current_position += line_dir * distance;
		}
	}

	void Spawn_Settings(ControlScript& Control,bool& leftclicked, int &Cube_ID, int& Cube2_ID, bool& start_Simulation, All_Potential_Type& potential_type)
	{
		if (ImGui::CollapsingHeader("Create Bodies"))
		{
			
			ImGui::InputFloat("Sphere raduis", &avg_raduis);
			ImGui::InputFloat("Line Size", &line_size);
			ImGui::InputFloat("Min Drift", &min_drift);
			ImGui::InputFloat("Min Distance", &min_distance);
			ImGui::InputFloat("real minimum distance", &real_min_distance);
			ImGui::Checkbox("Exclude Drift", &exclude_drift);
			ImGui::InputFloat("Collision Radius", &collision_raduis);
			ImGui::Checkbox("use center of mass", &use_center_mass);
			ImGui::Checkbox("limit velocity", &limit_velocity);
			ImGui::Checkbox("To Center", &potential_center);
			ImGui::Checkbox("Stereographic projection", &Stereographicprojection);
		}
		if (Control.External_Objects.size() > 0)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("BoundaryBox", ImVec2(0, 150), true, ImGuiWindowFlags_MenuBar);
			float pos3[3] = { Control.External_Objects[0]->transform.scale.x, Control.External_Objects[0]->transform.scale.y, Control.External_Objects[0]->transform.scale.z };
			ImGui::SeparatorText("Bounding Box");
			if (ImGui::InputFloat3("Size", pos3))
			{
				Control.External_Objects[0]->transform.scale.x = pos3[0];
				Control.External_Objects[0]->transform.scale.y = pos3[1];
				Control.External_Objects[0]->transform.scale.z = pos3[2];

			}
			float pos2[3] = { Control.External_Objects[0]->transform.position.x, Control.External_Objects[0]->transform.position.y, Control.External_Objects[0]->transform.position.z };
			if (ImGui::InputFloat3("position", pos2))
			{
				Control.External_Objects[0]->transform.position.x = pos2[0];
				Control.External_Objects[0]->transform.position.y = pos2[1];
				Control.External_Objects[0]->transform.position.z = pos2[2];

			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		if (ImGui::Button("Stereographic_projections"))
		{
			Stereographicprojection = true;
			for (int i = 2; i<Control.All_Meshs.size(); i++)
			{
				std::vector<Vertex> Sphere_vert;
				std::vector<unsigned int> Sphere_ind;
				Control.sphere.generateSphere(0.4, 5, 5, glm::vec3(1, 1, 0), Sphere_vert, Sphere_ind);
				mesh* obj = new mesh;
				glm::vec2 pos = stereographic_projection(Control.All_Meshs[i]->transform.position);
				obj->transform.position = glm::vec3(pos.x,0.0f,pos.y);
				obj->CreateMesh(Sphere_vert, Sphere_ind);
				Control.projections.push_back(obj);
				Control.All_Meshs[i]->SafePointIndex = Control.projections.size() - 1;
				real_min_distance = 1;
				potential_type = virtual_Potential;
				/*std::unique_ptr<mesh> line = std::make_unique<mesh>();
				std::vector<glm::vec3> Line;
				Line.push_back(Control.All_Meshs[i]->transform.position);
				Line.push_back(obj->transform.position);

				line->CreateLine(Line, glm::vec3(0.5,0.6,0.7));
				
				Control.projectionsline.push_back(std::move(line));*/
			}	
		}
		if (ImGui::Button("Plane debug"))
		{
			std::vector<Vertex> Sphere_vert;
			std::vector<unsigned int> Sphere_ind;
			Control.sphere.generatePlane(60, glm::vec3(0.1, 0.4, 0.6), Sphere_vert, Sphere_ind);
			Control.debugPlane->CreateMesh(Sphere_vert, Sphere_ind);
		}
		if (ImGui::Button("line_projections"))
		{
			glm::vec2 line_direction = glm::vec2(1.0f, 0.0f);
			align_points_with_fixed_distance(Control.projections, line_direction, 3);
		}
		
		if (ImGui::Checkbox("debug sphere", &debug_sphere))
		{
			if (debug_sphere)
			{
				std::vector<Vertex> Sphere_vert;
				std::vector<unsigned int> Sphere_ind;
				Control.sphere.generateSphere(real_min_distance, 15, 15, glm::vec3(1, 1, 0), Sphere_vert, Sphere_ind);
				Control.debugSphere->Poly_mode = 1;
				Control.debugSphere->CreateMesh(Sphere_vert, Sphere_ind);

				Control.debugSphere->transform.position = Control.All_Meshs[1]->transform.position;
			}else
			{
				Control.debugSphere->ClearMesh();
				//Control.debugSphere = nullptr;
			}

		}

		
	}

	void Tool_Bar(ControlScript& Control, int Cube_ID, bool& start_Simulation,int& Cube2_ID, bool& leftclicked)
	{
		bool isopen;
		ImGuiWindowFlags flag = 0;
		flag |= ImGuiWindowFlags_NoTitleBar;
		flag |= ImGuiWindowFlags_NoScrollbar;
		flag |= ImGuiWindowFlags_NoResize;
		ImGui::Begin("tool bar", &isopen,flag);
		int w, h;
		glfwGetWindowSize(Main_Window, &w, &h);

		ImGuiIO& io = ImGui::GetIO();
		ImTextureID my_tex_id = io.Fonts->TexID;
		float my_tex_w = (float)io.Fonts->TexWidth;
		float my_tex_h = (float)io.Fonts->TexHeight;
		ImGui::SetWindowPos(ImVec2(w / 2.5, 23));
		ImGui::SetWindowSize(ImVec2(250, 10));
		ImVec2 size = ImVec2(15.0f, 15.0f);                         // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);    // UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
		if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
		{
			ImGui::OpenPopup("draw mode");
		}
		if (ImGui::BeginPopup("draw mode"))
		{
			ImGui::SeparatorText("Wireframe Mode");
			bool A = false;
			bool B = false;
			bool C = false;
			switch (Control.All_Meshs[Cube_ID]->Poly_mode)
			{
			case 0:
				A = true;
				break;
			case 1:
				B = true;
				break;
			case 3:
				C = true;
				break;
			}
			if (ImGui::MenuItem("Fill", "", &A))
			{
				B = false; C = false; Control.All_Meshs[Cube_ID]->Poly_mode = 0;
			}
			if (ImGui::MenuItem("Point", "", &B))
			{
				A = false; C = false; Control.All_Meshs[Cube_ID]->Poly_mode = 1;
			}
			if (ImGui::MenuItem("Line", "", &C))
			{
				B = false; A = false; Control.All_Meshs[Cube_ID]->Poly_mode = 2;
			}


			ImGui::EndPopup();
		}
		ImGui::SameLine();


		ImGui::Checkbox("Simulate", &start_Simulation);
		ImGui::SameLine();

		if (Control.All_Meshs.size() > 0)
		{
			if (ImGui::Button("Reset"))
			{
				collide_number = 0;
				for (auto obj : Control.All_Meshs)
				{
					if (Control.All_Meshs[0] != obj)
					{
						obj->ClearMesh();
					}

				}
				for (auto obj : Control.projections)
				{
					if (Control.projections[0] != obj)
					{
						obj->ClearMesh();
					}

				}
				Stereographicprojection = false; 
				Fuel_Consumption.clear();
				Fuel_Time.clear();
				currentTime = 0;
				All_Vector_Sum.clear();
				All_vector_time.clear();
				Control.projections.clear();
				Control.projectionsline.clear();
				Control.All_Meshs.clear();
				start_Simulation = false;
				leftclicked = false;
				Cube_ID = 0;
				Cube2_ID = 0;
				Control.nextColorID = glm::vec4(0, 0, 0, 0);
				Control.ID = glm::vec4(0, 0, 0, 0);

				Control.t_data.clear();
				Control.x_data.clear();
				Control.y_data.clear();
				Control.z_data.clear();

				min_space_bound_check = FLT_MAX;
				cur_space_bound_check = 0;
				max_space_bound_check = FLT_MIN;
			}

		}
		

		ImGui::End();
	}

	
private:
	bool * Settings_window = new bool(false);
	bool * Analysis_window = new bool(false);
	int wid = 100;
	int hei = 100;


	bool checkmode(int index,int id)
	{
		if (index == id)return true;
		else return false;
	}
	bool * transparency = new bool(false);

	ImGuiWindowFlags window_flag = 0;
	
	void Pop_up_OVERLAY(ControlScript& Control)
	{
		
		
		if (Control.overlay_index > -1)
		{
			bool open = true;
			double x, y;
			
			glfwGetCursorPos(Main_Window, &x, &y);
			x = x + 10;
			y = y + 20;
			ImGuiWindowFlags Flags;
			Flags |= ImGuiWindowFlags_NoTitleBar;
			Flags |= ImGuiWindowFlags_NoResize;
			Flags |= ImGuiWindowFlags_NoScrollbar;
			ImGui::Begin("pop_up_bar", & open, Flags);
			//ImGui::SetWindowSize(ImVec2(0, 100));
			ImGui::SetWindowPos(ImVec2(x, y));
			//ImGui::BeginChild("overlaying", ImVec2(0, 0), ImGuiWindowFlags_NoScrollbar || ImGuiWindowFlags_NoResize);
			ImGui::SetWindowSize(ImVec2(0, 0));
			if (Control.overlay_index > 1)
			{
				ImGui::Text("..chaser..");
			}
			else
			{
				ImGui::Text("..Target..");
			}
			
			
			ImGui::Text("ID : \%d", Control.overlay_index);
			ImGui::Text("Group ID : \%d", Control.All_Meshs[Control.overlay_index]->GroupID);
			//ImGui::EndChild();
			//ImGui::PopStyleVar();
			ImGui::End();
		}
		
	}
	void ObjectTransforms(ControlScript& Control, int Cube_ID)
	{
		if (ImGui::CollapsingHeader("Satelite Transfrom")&& Control.All_Meshs.size()>0)
		{
			const char* name;
			switch (Cube_ID)
			{
				case 0:
					name = "Planet";
					break;
				case 1:
					name = "Target";
					break;
				default:
					name = "Chaser";
					break;
			}
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

			ImGui::BeginChild("Transforms", ImVec2(0, 200), true, ImGuiWindowFlags_MenuBar);
			float pos3[3] = { Control.All_Meshs[Cube_ID]->transform.position.x, Control.All_Meshs[Cube_ID]->transform.position.y, Control.All_Meshs[Cube_ID]->transform.position.z };
			ImGui::SeparatorText(name);
			if (ImGui::InputFloat3("position", pos3))
			{
				Control.All_Meshs[Cube_ID]->transform.position.x = pos3[0];
				Control.All_Meshs[Cube_ID]->transform.position.y = pos3[1];
				Control.All_Meshs[Cube_ID]->transform.position.z = pos3[2];

			}
			float pos5[3] = { Control.All_Meshs[Cube_ID]->transform.rotation.x, Control.All_Meshs[Cube_ID]->transform.rotation.y, Control.All_Meshs[Cube_ID]->transform.rotation.z };
			if (ImGui::InputFloat3("rotation", pos5))
			{
				Control.All_Meshs[Cube_ID]->transform.rotation.x = pos5[0];
				Control.All_Meshs[Cube_ID]->transform.rotation.y = pos5[1];
				Control.All_Meshs[Cube_ID]->transform.rotation.z = pos5[2];

			}
			float rad = Control.All_Meshs[Cube_ID]->transform.raduis;
			if (ImGui::InputFloat("Raduis", &rad))
			{
				if (rad < 10.1)
				{
					Control.All_Meshs[Cube_ID]->transform.raduis = rad;
					Control.All_Meshs[Cube_ID]->transform.scale = Control.All_Meshs[Cube_ID]->transform.scale * Control.All_Meshs[Cube_ID]->transform.raduis;
				}

			}
			float vec3[3] = { Control.All_Meshs[Cube_ID]->transform.velocity.x, Control.All_Meshs[Cube_ID]->transform.velocity.y, Control.All_Meshs[Cube_ID]->transform.velocity.z };

			if (ImGui::InputFloat3("velocity", vec3))
			{
				Control.All_Meshs[Cube_ID]->transform.velocity.x = vec3[0];
				Control.All_Meshs[Cube_ID]->transform.velocity.y = vec3[1];
				Control.All_Meshs[Cube_ID]->transform.velocity.z = vec3[2];

			}
			if (ImGui::InputFloat3("force", vec3))
			{
				Control.All_Meshs[Cube_ID]->transform.force.x = vec3[0];
				Control.All_Meshs[Cube_ID]->transform.force.y = vec3[1];
				Control.All_Meshs[Cube_ID]->transform.force.z = vec3[2];

			}


			if (ImGui::InputFloat("mass", &Control.All_Meshs[Cube_ID]->transform.mass))
			{

			}
			ImGui::EndChild();
			ImGui::PopStyleVar();

		}
	}
};
#endif
