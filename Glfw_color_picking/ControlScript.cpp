#include "ControlScript.h"
glm::vec3 ControlScript::RandomPosition()
{
	float x = ((float)rand() / (float)RAND_MAX) * 15.0f - 5.0f;
	float y = ((float)rand() / (float)RAND_MAX) * 15.0f - 5.0f;
	float z = ((float)rand() / (float)RAND_MAX) * 15.0f - 5.0f;

	return glm::vec3(x, y, z);
}

void ControlScript::Int_ControlScript()
{
	initialdata.openLogFile("InitialValues.txt");
	std::ifstream file("InitialValues.txt");
	//sphere.generateSphere(avg_raduis, 15, 15, glm::vec3(1.0f, 0.1f, 1.0f), Sphere_vert, Sphere_ind);
	sphere.generateTriangle(glm::vec3(1.0f, 0.1f, 1.0f), Sphere_vert, Sphere_ind);
	int index = -1;
	std::string line;
	float value1, value2, value3;
	std::vector<glm::vec3> random_vectors;
	if (UseRandomValueFromFile)
	{
		while (std::getline(file, line))
		{ // Correct usage of std::getline
			index++;
			if (index != 0)
			{
				std::istringstream iss(line);
				iss >> value1 >> value2 >> value3;
				random_vectors.push_back(glm::vec3(value1, value2, value3));
				//std::cout << "Value : " << value1 << "\t" << "Value : " << value2 << "\t" << "Value : " << value3 << "\n";
			}
			//std::cout << line << std::endl;
			if (index == SwarmSize * 2)
			{
				break;
			}
		}
	}
	int val=-1;
	for (int i = 0; i < SwarmSize; i++)
	{
		
		
		mesh* obj = new mesh;
		obj->CreateMesh(Sphere_vert, Sphere_ind);
		if (!UseRandomValueFromFile)
		{
			obj->transform.position = RandomPosition();
			obj->transform.velocity = RandomPosition() / 20.0f;
		}
		else
		{
			val++;
			obj->transform.position = random_vectors[val];
			val++;
			obj->transform.velocity = random_vectors[val];
		}

		if (All_Meshs.size() < 2)
		{
			obj->transform.position = glm::vec3(0, 0, 0);
			
			obj->transform.velocity = glm::vec3(0, 0, 0);
			obj->UpdateColor(glm::vec3(0, 0, 1));
		}
		else
		{
			initialdata.logvalues(std::to_string(obj->transform.position.x), std::to_string(obj->transform.position.y), std::to_string(obj->transform.position.z));
			initialdata.logvalues(std::to_string(obj->transform.velocity.x), std::to_string(obj->transform.velocity.y), std::to_string(obj->transform.velocity.z));
		}
		
		obj->ColorID = uniqueID();
		if (obj->ColorID == Background_Color)
		{
			obj->ColorID = uniqueID();
		}
		All_Meshs.push_back(obj);

	}
	
	initialdata.closeLogFile();
	//std::vector<glm::vec3> line;
	//generateEllipse(line, 7, 7, 500);
	//midptellipse(7, 5, 5, 5, line);
	//target_orbit.CreateLine(line, glm::vec3(0.8, 0.1, 0.5));


	//All_Meshs[1]->transform.position = glm::vec3(5, 5, 5);
	//All_Meshs[1]->transform.velocity = glm::vec3(0.01, 0.02,0.03);
	//All_Meshs[0]->transform.position = glm::vec3(0, 0, 0);
	//All_Meshs[0]->transform.velocity = glm::vec3(0.00, 0.00, 0.00);

}
ControlScript::~ControlScript()
{
	for (auto obj : All_Meshs)
	{
		delete obj;
	}
	for (auto obj : All_TraceLines)
	{
		delete obj;
	}
	for (auto obj : External_Objects)
	{
		delete obj;
	}
	for (auto obj : projections)
	{
		delete obj;
	}

	if (debugSphere)
	{
		delete debugSphere;
	}
	delete eliptic_orbit;
	if (debugPlane)
	{
		delete debugPlane;
	}
}
void ControlScript::SetGroups(GLuint& shaderProgram, glm::mat4& CamMatrix)
{
	std::vector<int>temp;
	for (int i = 2; i < All_Meshs.size(); i++)
	{
		temp.push_back(i);
	}
	adjacency_matrix(temp);
	int grpNum = -1;
	for (auto obj : All_Meshs)
	{																					 
		obj->GroupID = -1;
		obj->potential_distances = glm::vec4(-1,0,0,0);
	}
	for (int i = 0; i < All_lines.size(); i++)
	{
		All_lines[i]->ClearMesh();
	}
	Groups.clear();
	All_lines.clear();
	while (temp.size() > 0)
	{
		++grpNum;
		if (Groups.size() < grpNum + 1)
		{
			Groups.resize(grpNum + 1);
		}
		for (int i = temp.size() - 1; i > -1; --i)
		{
			if (i == 0)
			{
				Groups[grpNum].push_back(All_Meshs[temp[i]]);
				All_Meshs[temp[i]]->GroupID = grpNum;
				temp.erase(temp.begin() + i);
			}
			else if (CompareMesh(All_Meshs[temp[0]], All_Meshs[temp[i]]))
			{
				
				All_Meshs[temp[i]]->GroupID = grpNum;
				Groups[grpNum].push_back(All_Meshs[temp[i]]);
				temp.erase(temp.begin() + i);
			}

		}

	}

	for (int i = 0; i < Groups.size(); i++)
	{
		for (int j = 0; j < Groups[i].size(); j++)
		{
			Groups[i][j]->GroupID = i;
		}
	}
	int MaxGrp = -1;
	int MaxSize = 0;
	for (int i = 0; i < Groups.size(); i++)
	{
		if (Groups[i].size() > 1)
		{
			if (Groups[i].size() > MaxSize)
			{
				MaxSize = Groups[i].size();
				MaxGrp = i;
			}
		}
	}


	for (int i = 0; i < Groups.size(); i++)
	{
		for (int j = 0; j < Groups[i].size(); j++)
		{
			if (MaxGrp == i)
			{
				switch (linetype)
				{
				case 0:
					Original_Al(Groups[i], glm::vec3(0, 1, 0));
					break;
				case 1:
					Dijkstra_Al(Groups[i], glm::vec3(0, 1, 0));
					break;
				case 2:
					Kruskals_Al(Groups[i], glm::vec3(0, 1, 0));
					break;
				}
				
				
			}
			else if (Groups[i].size() > 1)
			{

				switch (linetype)
				{
				case 0:
					Original_Al(Groups[i], glm::vec3(0, 0, 1));
					break;
				case 1:
					Dijkstra_Al(Groups[i], glm::vec3(0, 0, 1));
					break;
				case 2:
					Kruskals_Al(Groups[i], glm::vec3(0, 0, 1));
					break;
				}

				if (MaxGrp != -1)
				{
					NonSingleToLarget(Groups[i], Groups[MaxGrp], glm::vec3(1, 1, 1));
				}
			}
			else
			{
				if (MaxGrp != -1)
				{
					SingleToLargest(Groups[i][0], Groups[MaxGrp], glm::vec3(1, 0, 0));
				}
				
			}
		}
	}
	if (All_lines.size() < 1)
	{
		NoGroups = true;
	}
	else
	{
		NoGroups = false;
	}
	ClohessyWhilshire();
	UpdateProjectionLines();
	if (Connecting_Line)
	{
		for (int i = 0; i < All_lines.size(); i++)
		{
			All_lines[i]->DrawLine(shaderProgram, CamMatrix);
		}
	}

}
void ControlScript::ClohessyWhilshire()
{
	for (int i = 0; i < All_Meshs.size(); i++)
	{
		All_Meshs[i]->transform.drift = All_Meshs[i]->transform.velocity.y + 2.0f * All_Meshs[i]->transform.position.x;
	}
}


void ControlScript::Add_Body()
{
	sphere.generateSphere(avg_raduis, 15, 15, glm::vec3(1.0f, 0.1f, 1.0f), Sphere_vert, Sphere_ind);
	mesh* obj = new mesh;
	
	obj->transform.position = RandomPosition();
	obj->transform.velocity = RandomPosition() / 20.0f;
	if (All_Meshs.size() < 2)
	{
		obj->transform.position = glm::vec3(0, 0, 0);
		obj->transform.velocity = glm::vec3(0, 0, 0);
		obj->UpdateColor(glm::vec3(0, 0, 1));
	}
	obj->CreateMesh(Sphere_vert, Sphere_ind);
	obj->ColorID = uniqueID();
	if (obj->ColorID == Background_Color)
	{
		obj->ColorID = uniqueID();
	}
	
	All_Meshs.push_back(obj);
}
