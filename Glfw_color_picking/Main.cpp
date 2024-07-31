#include <iostream>
#include "Glew_Initialization.h"
#include "shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "ColorPicking.h"
#include "Sphere.h"
#include "PhysicsHandler.h"
#include "ManageUI.h"
#include "ControlScript.h"
#include "ScriptingArea.h"
#include "logger.h"

bool start_Simulation = false;
bool IsWidget;
PID pid;
Shaders shader; // main shader program
Shaders pickingshader; // picking color shader program

GenSphere c_Sphere;//generate sphre vertices

Color_Picking colorPicking; 

Glew_Initialization sys;
All_Potential_Type potential_type;
std::vector<Vertex> cube_vertex =
{
	// positions          // colors           // normals         // texUV
 { glm::vec3(-0.3,-0.3,-0.3), glm::vec3(1.0,0.0,0.5), glm::vec3(-1,-1,-1), glm::vec2(0, 0) },
 { glm::vec3(-0.3,-0.3,0.3), glm::vec3(1.0,0.0,0.5), glm::vec3(-1,-1, 1), glm::vec2(1, 0) },
 { glm::vec3(-0.3,0.3,-0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(-1, 1,-1), glm::vec2(0, 1) },
 { glm::vec3(-0.3,0.3,0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(-1, 1, 1), glm::vec2(1, 1) },
 { glm::vec3(0.3,-0.3,-0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(1,-1,-1),  glm::vec2(1, 0) },
 { glm::vec3(0.3,-0.3,0.3),   glm::vec3(1.0,0.0,0.5), glm::vec3(1,-1, 1),  glm::vec2(0, 0) },
 { glm::vec3(0.3,0.3,-0.3),  glm::vec3(1.0,0.0,0.5) , glm::vec3(1, 1,-1),  glm::vec2(1, 1) },
 { glm::vec3(0.3,0.3,0.3),    glm::vec3(1.0,0.0,0.5), glm::vec3(1, 1, 1),  glm::vec2(0, 1) }
};
std::vector<unsigned int> indices =
{
   0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, // front 
   1, 5, 3, 5, 3, 7, 0, 4, 2, 4, 2, 6, // back 
   0, 1, 4, 1, 4, 5, 2, 3, 6, 3, 6, 7  // sides
};


std::vector<std::unique_ptr<mesh>> All_lines;

Eigen::MatrixXd Kk;

glm::mat4 CamMatrix; //camera Matrix

void Render_Scene();
void Render_Picking_Scene();
int  Get_Object(glm::vec4 ColorID);
bool Almost_Equal(glm::vec4 a, glm::vec4 b);
void InitiatePicking();
void InitiateOverlayPicking();
void ClearUp();
void CheckIfWidget(ImGuiIO& io)
{
	if (io.WantCaptureMouse || ImGui::IsAnyItemHovered())
	{
		IsWidget = true;
	}
	else
	{
		IsWidget = false;
	}
}


bool leftclicked = false;
int Cube_ID = 0;
int Cube2_ID = 0;
Script_Editor pyIntergrate;

ControlScript Control;
Manage_Widgets ui;
Log dispersion;
Log sat_1;
Log sat_2;
Log sat_3;
Log sat_4;
Log Fuel_comsume;
double totalTime = 500.0;
int main()
{
	
//#if defined(WIN32) || defined(_WIN32)
//	_putenv_s("PYTHONHOME", "C:/Users/ndene/AppData/Local/Programs/Python/Python39");
//#else
//	setenv("PYTHONHOME", "C:/Users/ndene/AppData/Local/Programs/Python/Python39", 1);
//#endif
	//PhysicsHandler::LQR_Solver(Kk, 1, 1,- 10000);
	//std::cout << "kk" << Kk;
	sys.Init_Glfw();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(Main_Window , true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("../ExternalLibs/freedom-font/VTCSundaykomix.ttf", 18.5f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Users/User/Documents/Visual Studio 2022/Projects/Hakujitsu _v2/Hakujitsu/ExternalLibs/freedom-font/VIPRoman-Regular.ttf", 18.5f);
	if (font)
	{
		std::cout << "Font loaded" << "\n";
	}
	else
	{
		std::cout << "Font failed to load" << "\n";
	}
	io.FontDefault = font;
	Camera camera;

	shader.Create_Normal_Shaders();
	pickingshader.Create_Picking_Shaders();
	
	// create custome framebuffer
	colorPicking.Init(sys.width, sys.height, glm::vec3(0, 0, 0));
	std::vector<Vertex> Sphere_vert;
	std::vector<unsigned int> Sphere_ind;

	
	//create cubes and assign unique ColorIDs
	c_Sphere.generateSphere(0.5f, 10, 10, glm::vec3(1, 0, 0), Sphere_vert, Sphere_ind);

	mesh cord_x;
	mesh cord_y;
	mesh cord_z;
	std::vector<glm::vec3> xcord = { glm::vec3(0,0,0),glm::vec3(10,0,0) };
	std::vector<glm::vec3> ycord = { glm::vec3(0,0,0),glm::vec3(0,10,0) };
	std::vector<glm::vec3> zcord = { glm::vec3(0,0,0),glm::vec3(0,0,10) };

	cord_x.CreateLine(xcord, glm::vec3(0.5, 0, 0.5));
	cord_y.CreateLine(ycord, glm::vec3(0, 0.5, 1));
	cord_z.CreateLine(zcord, glm::vec3(0.5,0,1));


	Control.Add_Body();
	Control.All_Meshs[0]->UpdateColor(glm::vec3(0, 1, 0));
	Control.All_Meshs[0]->transform.position = glm::vec3(0, 0, 0);
	Control.All_Meshs[0]->transform.raduis = 10;
	Control.All_Meshs[0]->transform.rotation = glm::vec3(0, 0, -23.44);

	Control.All_Meshs[0]->gen_line();
	
	Control.All_Meshs[0]->transform.scale = Control.All_Meshs[0]->transform.scale * Control.All_Meshs[0]->transform.raduis;
	
	
	glClearColor(Background_Color[0], Background_Color[1], Background_Color[2], Background_Color[3]);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	/*GLfloat lineWidthRange[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);

	GLfloat lineWidthGranularity;
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &lineWidthGranularity);

	std::cout << "Supported line width range: [" << lineWidthRange[0] << ", " << lineWidthRange[1] << "]" << std::endl;
	std::cout << "Line width granularity: " << lineWidthGranularity << std::endl;*/
	
	potential_type = Harmonic_Potential;
	dispersion.openLogFile("dispersion.txt");
	sat_1.openLogFile("sat_1.txt");
	sat_2.openLogFile("sat_2.txt");
	sat_3.openLogFile("sat_3.txt");
	sat_4.openLogFile("sat_4.txt");
	Fuel_comsume.openLogFile("fuel_consumed.txt");
	while (!glfwWindowShouldClose(Main_Window ))
	{
		CheckIfWidget(io);
		glfwPollEvents();
		CamMatrix = camera.Camera_Matrix();
		camera.Camera_Controls(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		Control.SetGroups(shader.shaderprogram, CamMatrix);
		cord_x.DrawLine(shader.shaderprogram, CamMatrix);
		cord_y.DrawLine(shader.shaderprogram, CamMatrix);
		cord_z.DrawLine(shader.shaderprogram, CamMatrix);
		Render_Scene();
		//Render_Picking_Scene();
		if (Control.All_Meshs.size() > 0)
		{
			
			InitiatePicking();
			InitiateOverlayPicking();
		}
		
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ui implementation
		
		ui.manage(Control,leftclicked,Cube_ID,Cube2_ID,start_Simulation, pyIntergrate, potential_type);
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(Main_Window );
		

	}
	
	ClearUp();
	glfwDestroyWindow(Main_Window );
	glfwTerminate();
	dispersion.closeLogFile();
	sat_1.closeLogFile();
	sat_2.closeLogFile();
	sat_3.closeLogFile();
	sat_4.closeLogFile();
	Fuel_comsume.closeLogFile();
}


void Render_Scene()
{
	if (start_Simulation)
	{
		//PhysicsHandler::Calc_Omega(Control.All_Meshs);
		//pyIntergrate.RunScript(omega);
		PhysicsHandler::Forces(Control.All_Meshs, pyIntergrate,Control.t_data, Control.x_data, Control.y_data, Control.z_data,pid, potential_type,Control, dispersion,sat_1, sat_2, sat_3, sat_4);
		PhysicsHandler::Center_Revolution(Control.All_Meshs[0]); // Total time in seconds
		// Time step in seconds

		double totalDistance = PhysicsHandler::calculateFuelConsumption(Control.All_Meshs, totalTime, dt);
		currentTime = currentTime + dt;
		
		// Assuming a simple linear relationship between distance and fuel consumption
		double fuelConsumptionPerUnitDistance = 0.1; // Example value
		double totalFuelConsumption = totalDistance * fuelConsumptionPerUnitDistance;
		Fuel_Consumption.push_back(totalDistance);
		Fuel_Time.push_back(currentTime);
		std::string time = std::to_string(currentTime);
		std::string value = std::to_string(totalDistance);
		Fuel_comsume.logMessage(time, value);
		if (currentTime > totalTime+5500)
		{
			//start_Simulation = false;
			//exit(0);
		}
	}
	//PhysicsHandler::Avoidance(Control.All_Meshs);
	//for (int i = 0; i < Control.All_Meshs.size(); i++)
	//{
	//	PhysicsHandler::Step(Control.All_Meshs[i]->transform);
	//}

	for (int i = 1; i < Control.All_Meshs.size(); i++)
	{
		if (start_Simulation)
		{
			PhysicsHandler::Step(Control.All_Meshs[i]->transform);
		}

	}
	for (int i = 0;i<Control.External_Objects.size();i++)
	{
		Control.External_Objects[i]->RenderMesh(shader.shaderprogram, CamMatrix);
		if (i == 0)
		{
			boundary_limits =  Control.External_Objects[i]->transform.scale;
		}
	}
	if (Control.External_Objects.size() <= 0)
	{
		boundary_limits = glm::vec3(0, 0, 0);
	}

	if (debug_sphere)
	{
		Control.debugSphere->RenderMesh(shader.shaderprogram, CamMatrix);
	}
	Control.debugPlane->RenderMesh(shader.shaderprogram, CamMatrix);
	
	if (Control.All_Meshs.size() > 1)
	{
		Control.target_orbit.DrawLine(shader.shaderprogram, CamMatrix);// target orbit
		//PhysicsHandler::Target_Orbit(Control.All_Meshs[0], Control.target_orbit.ellipse);

	}
	if(Control.eliptic_orbit->move_on_elipse)
	{
		Control.eliptic_orbit->DrawLine(shader.shaderprogram, CamMatrix);
	}
	
	for (auto obj : Control.All_TraceLines)
	{
		if (obj->ShowLine)
		{
			obj->DrawLine(shader.shaderprogram, CamMatrix);
		}
		
	}

	for (int i =0; i<Control.projectionsline.size();i++)
	{
		
		Control.projectionsline[i]->DrawLine(shader.shaderprogram, CamMatrix);
		

	}


	for (int i = 0;i<Control.All_Meshs.size();i++)
	{
		Control.All_Meshs[i]->RenderMesh(shader.shaderprogram, CamMatrix);
		Control.All_Meshs[i]->currentIndex = i;
	}

	for (auto obj : Control.projections)
	{
		obj->RenderMesh(shader.shaderprogram, CamMatrix);
	}
	if (Control.All_Meshs.size() > 0)
	{
		if (Control.All_Meshs[0]->show_equator_line)
		{
			Control.All_Meshs[0]->DrawLine(shader.shaderprogram, CamMatrix);
			Control.All_Meshs[0]->DrawAxis(shader.shaderprogram, CamMatrix);
		}
	}
}
void Render_Picking_Scene()
{
	for (int i = 0; i < Control.All_Meshs.size(); i++)
	{
		int index = Control.All_Meshs[i]->Poly_mode;
		Control.All_Meshs[i]->Poly_mode = 0;
		Control.All_Meshs[i]->RenderMesh(pickingshader.shaderprogram, CamMatrix);
		Control.All_Meshs[i]->Poly_mode = index;
	}
}
void InitiatePicking()
{
	if (glfwGetMouseButton(Main_Window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !IsWidget)
	{
		if (!leftclicked)
		{
			Render_Scene();
			Render_Picking_Scene();
			//draw cubes in the custom frame buffer
			colorPicking.EnableWriting();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			Render_Picking_Scene();
			colorPicking.DisableWriting();
			//read color values from custom frame buffer
			Cube_ID = Get_Object(colorPicking.Read_Pixels());

			Control.All_Meshs[Cube_ID]->UpdateColor(glm::vec3(0.9f,0.9f,0.9f));

			for (int i = 0; i < Control.All_Meshs.size(); i++)
			{
				if (i != Cube_ID)
				{
					Control.All_Meshs[i]->UpdateColor(Control.All_Meshs[i]->Color);
				}
			}
			leftclicked = true;
		}

	}
	else if (glfwGetMouseButton(Main_Window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (leftclicked)
		{
			leftclicked = false;
		}

	}
}
void InitiateOverlayPicking()
{
	if (can_read_Overlay && !IsWidget)
	{
		Render_Picking_Scene();
		//draw cubes in the custom frame buffer
		colorPicking.EnableWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Render_Picking_Scene();
		colorPicking.DisableWriting();
		//read color values from custom frame buffer
		if (colorPicking.Read_Pixels() == Background_Color)
		{
			Control.overlay_index = -1;
		}
		else
		{
			Control.overlay_index = Get_Object(colorPicking.Read_Pixels());
		}

	}

}
int Get_Object(glm::vec4 ColorID)
{
	int e = 0;
	for (int i = 0; i < Control.All_Meshs.size(); i++)
	{
		if (Almost_Equal(Control.All_Meshs[i]->ColorID, ColorID))
		{
			e = i;
			break;

		}
	}
	if (ColorID == Background_Color)
	{
		return Cube_ID;
	}
	else
	{
		return e;
	}


}
bool Almost_Equal(glm::vec4 a, glm::vec4 b)
{
	double epsilon = 0.0001;
	bool check_diff = true;
	for (int i = 0; i < 4; i++)
	{
		if (std::abs(a[i] - b[i]) > epsilon)
		{
			check_diff = false;
		}
	}
	return check_diff;
}
void ClearUp()
{
	for (auto obj : Control.All_Meshs)
	{
		obj->ClearMesh();
	}
	for (auto obj : Control.All_TraceLines)
	{
		obj->ClearMesh();
	}
	
	ImPlot::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
}
