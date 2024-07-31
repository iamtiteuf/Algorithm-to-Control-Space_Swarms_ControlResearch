#ifndef CONTROL_SCRIPT_H
#define CONTROL_SCRIPT_H
#include<glm/glm.hpp>
#include "Mesh.h"
#include "LineTrace.h"
#include "Sphere.h"
#include <algorithm>
#include <queue>
#include "logger.h"
class ControlScript
{
public:
	ControlScript()
	{
		linetype = Original;
	}
	std::vector<mesh*> All_Meshs;
	std::vector<lineT*> All_TraceLines;
	std::vector<mesh*> External_Objects;
	std::vector<mesh*> projections;
	std::vector<std::unique_ptr<mesh>> projectionsline;
	mesh target_orbit;
	GenSphere sphere;
	mesh* eliptic_orbit = new mesh;
	std::vector<std::unique_ptr<mesh>> All_lines;
	std::vector<std::vector<mesh*>> Groups;
	mesh* debugSphere = new mesh;
	mesh* debugPlane = new mesh;
	Eigen::MatrixXd adjacency_mat;

	void Int_ControlScript();
	~ControlScript();
	void SetGroups(GLuint& shaderProgram, glm::mat4& CamMatrix);
	void ClohessyWhilshire();
	void Add_Body();
	void Add_Bounding_Box()
	{
		mesh* obj = new mesh;
		sphere.generateBox(glm::vec3(0, 0, 1), Sphere_vert, Sphere_ind);
		obj->CreateMesh(Sphere_vert, Sphere_ind);
		obj->transform.position = glm::vec3(0, 0, 0);
		obj->transform.velocity = glm::vec3(0, 0, 0);
		obj->Poly_mode = 2;
		External_Objects.push_back(obj);
	}
	glm::vec4 nextColorID = glm::vec4(0, 0, 0, 0);
	glm::vec4 ID = glm::vec4(0, 0, 0, 0);
	bool NoGroups = false;
	bool LineAlgo = false;
	Linetype linetype;
	std::vector<float> t_data;
	std::vector<float> y_data;
	std::vector<float> z_data;
	std::vector<float> x_data;
	int overlay_index = -1;
private:
	Log initialdata;

	bool fileExists(const std::string& filename) 
	{
		std::ifstream file(filename.c_str());
		return file.good();
	}
	void readFile(const std::string& filename, std::vector<glm::vec3>& values) 
	{
		std::ifstream file(filename);
		if (!file.is_open()) return;

		std::string line;
		
	}
	void writeFile(const std::string& filename, const std::vector<glm::vec3>& values) {
		std::ofstream file(filename);
		if (!file.is_open()) return;

		
	}
	static bool CompareMesh(mesh* a, mesh* b)
	{
		if (exclude_drift)
		{
			if ( glm::distance(a->transform.position, b->transform.position) < min_distance)
			{
				
				
				return true;
			}
			else
			{
				return false;  // or true, depending on your desired ordering
			}
		}
		else
		{
			if (std::abs(a->transform.drift - b->transform.drift) < min_drift && glm::distance(a->transform.position, b->transform.position) < min_distance)
			{
				return true;
			}
			else
			{
				return false;  // or true, depending on your desired ordering
			}
		}
		
	}
	void SingleToLargest(mesh* a, std::vector<mesh*>group,glm::vec3 Color)
	{

		glm::vec3 center_mass = glm::vec3(0,0,0);
	

		float distance = FLT_MAX;
		int index = 0;
		for (int i = 0; i < group.size(); i++)
		{
			if (glm::distance(a->transform.position, group[i]->transform.position) < distance)
			{
				distance = glm::distance(a->transform.position, group[i]->transform.position);
				index = i;
			}
			center_mass.x += group[i]->transform.position.x;
			center_mass.y += group[i]->transform.position.y;
			center_mass.z += group[i]->transform.position.z;
		}
		center_mass = center_mass /(float)group.size();
		std::unique_ptr<mesh> line = std::make_unique<mesh>();
		std::vector<glm::vec3> Line;
		Line.push_back(a->transform.position);
		Line.push_back(group[index]->transform.position);
		//float dis = glm::distance(a->transform.position, group[index]->transform.position);

		if (use_center_mass)
		{
			a->potential_distances = glm::vec4(real_min_distance, center_mass);

		}
		else
		{
			a->grp_mates.clear();
			a->grp_mates.push_back(group[index]->transform.position);
			a->potential_distances = glm::vec4(real_min_distance, group[index]->transform.position);
		}
		
		line->CreateLine(Line, Color);
		All_lines.push_back(std::move(line));
	}
	void NonSingleToLarget(std::vector<mesh*>a, std::vector<mesh*>b,glm::vec3 Color) //a single, b largest
	{
		glm::vec3 center_mass = glm::vec3(0, 0, 0);
		float distance = FLT_MAX;
		int Aindex = 0;
		int Bindex = 0;
		for (int i = 0; i < a.size(); i++)
		{
			for (int j = 0; j < b.size(); j++)
			{
				if (glm::distance(a[i]->transform.position, b[j]->transform.position) < distance)
				{
					distance = glm::distance(a[i]->transform.position, b[j]->transform.position);
					Aindex = i;
					Bindex = j;
				}
				center_mass.x += b[j]->transform.position.x;
				center_mass.y += b[j]->transform.position.y;
				center_mass.z += b[j]->transform.position.z;
			}
			
		}
		center_mass = center_mass / (float)b.size();
		std::unique_ptr<mesh> line = std::make_unique<mesh>();
		std::vector<glm::vec3> Line;
		Line.push_back(a[Aindex]->transform.position);
		Line.push_back(b[Bindex]->transform.position);

		if (use_center_mass)
		{
			a[Aindex]->potential_distances = glm::vec4(real_min_distance, center_mass);

		}
		else
		{
			a[Aindex]->potential_distances = glm::vec4(real_min_distance, b[Bindex]->transform.position);
			a[Aindex]->grp_mates.push_back(b[Bindex]->transform.position);
		}
		
		/*for (int i = 0; i < a.size(); i++)
		{
			if (i != Aindex)
			{
				float dis = glm::distance(a[Aindex]->transform.position, a[i]->transform.position);
				a[i]->potential_distances = glm::vec4(real_min_distance, a[Aindex]->transform.position);
				
			}
		}*/


		line->CreateLine(Line, Color);
		All_lines.push_back(std::move(line));
	}
	
	void adjacency_matrix(std::vector<int> index)
	{
		adjacency_mat.resize(index.size(), index.size());
		for (int i = 0; i < index.size(); i++)
		{
			for (int j = 0; j < index.size(); j++)
			{
				if (i == j)
				{
					adjacency_mat(i, j) = 0;
				}
				else
				{
					if (CompareMesh(All_Meshs[index[i]], All_Meshs[index[j]]))
					{
						adjacency_mat(i, j) = 1;
					}
					else
					{
						adjacency_mat(i, j) = 0;
					}

				}
				
			}
		}
	}

	void Kruskals_Al(std::vector<mesh*> a, glm::vec3 Color) {
		std::vector<Edge> edges;
		for (int i = 0; i < a.size(); i++) {
			for (int j = i + 1; j < a.size(); j++) {
				float distance = glm::distance(a[i]->transform.position, a[j]->transform.position);
				edges.push_back(Edge(i, j, distance));
			}
		}

		std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
			return a.distance < b.distance;
			});

		std::vector<int> parent(a.size());
		for (int i = 0; i < a.size(); i++) {
			parent[i] = i;
		}

		std::vector<Edge> mst;
		for (const Edge& edge : edges) {
			int parent1 = find(parent, edge.vertex1);
			int parent2 = find(parent, edge.vertex2);

			if (parent1 != parent2) {
				mst.push_back(edge);
				union_(parent, parent1, parent2);
			}
		}

		for (const Edge& edge : mst) {
			std::unique_ptr<mesh> line = std::make_unique<mesh>();
			std::vector<glm::vec3> Line;
			Line.push_back(a[edge.vertex1]->transform.position);
			Line.push_back(a[edge.vertex2]->transform.position);
			line->CreateLine(Line, Color);
			All_lines.push_back(std::move(line));
		}
	}

	void Dijkstra_Al(std::vector<mesh*> a, glm::vec3 Color) 
	{
		std::vector<std::pair<mesh*, mesh*>> Pairs;
		std::vector<mesh*> remainingMeshes = a;
		std::vector<mesh*> connectedMeshes;
		float MaxLeft = FLT_MAX;
		int start = -1;

		// Find the leftmost mesh to start
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i]->transform.position.x < MaxLeft)
			{
				MaxLeft = a[i]->transform.position.x;
				start = i;
			}
		}

		// Start with the leftmost mesh
		connectedMeshes.push_back(a[start]);
		remainingMeshes.erase(remainingMeshes.begin() + start);

		while (!remainingMeshes.empty())
		{
			float minDistance = FLT_MAX;
			int nearestIndex = -1;

			// Find the nearest mesh to the last connected mesh
			for (int i = 0; i < remainingMeshes.size(); i++)
			{
				float dist = glm::distance(connectedMeshes.back()->transform.position, remainingMeshes[i]->transform.position);
				if (dist < minDistance)
				{
					minDistance = dist;
					nearestIndex = i;
				}
			}

			// Connect the nearest mesh
			if (nearestIndex > -1)
			{
				Pairs.push_back({ connectedMeshes.back(), remainingMeshes[nearestIndex] });
				connectedMeshes.push_back(remainingMeshes[nearestIndex]);
				remainingMeshes.erase(remainingMeshes.begin() + nearestIndex);
			}
		}

		// Draw lines
		for (const auto& pair : Pairs)
		{
			std::unique_ptr<mesh> line = std::make_unique<mesh>();
			std::vector<glm::vec3> Line;
			Line.push_back(pair.first->transform.position);
			Line.push_back(pair.second->transform.position);
			line->CreateLine(Line, Color);
			All_lines.push_back(std::move(line));

			if (pair.first->currentIndex >= 0 && pair.first->currentIndex < a.size() &&
				pair.second->currentIndex >= 0 && pair.second->currentIndex < a.size())
			{
				a[pair.second->currentIndex]->potential_distances = glm::vec4(
					min_distance,pair.first->transform.position.x,pair.first->transform.position.y,pair.first->transform.position.z
				);
			}

			/*if (ApplyPotential_To_One)
			{
				a[i]->potential_distances = glm::vec4(real_min_distance, a[b]->transform.position);
			}*/
		}
	}

	void Original_Al(std::vector<mesh*> a, glm::vec3 Color)
	{
		int b = 0;
		glm::vec3 center_mass = glm::vec3(0,0,0);
		for (int i = 0; i < a.size(); i++)
		{
			center_mass.x += a[i]->transform.position.x;
			center_mass.y += a[i]->transform.position.y;
			center_mass.z += a[i]->transform.position.z;
		}
		center_mass = center_mass / (float)a.size();

		for (int i = 0; i < a.size()-1; i++)
		{
			mesh* x = a[i];
			mesh* y = nullptr;
			double minDistance = std::numeric_limits<double>::max();
			a[i]->grp_mates.clear();
			for (int j = i+1; j < a.size(); j++)
			{
				if (i != j)
				{
					double distance = glm::distance(x->transform.position, a[j]->transform.position);
					if (distance < minDistance) {
						minDistance = distance;
						y = a[j];
						b = j;
					}
					
					if (!ApplyPotential_To_One)
					{
						a[i]->potential_distances = glm::vec4(real_min_distance, a[j]->transform.position);
						a[i]->grp_mates.push_back(a[j]->transform.position);
					}
				}
			}

			if (y != nullptr)
			{
				std::unique_ptr<mesh> bline = std::make_unique<mesh>();
				std::vector<glm::vec3> Line;
				Line.push_back(x->transform.position);
				Line.push_back(y->transform.position);
				bline->CreateLine(Line, Color); //draw white line
				All_lines.push_back(std::move(bline)); // Add the unique_ptr to AllLines

				
				
				/*if (ApplyPotential_To_One)
				{
					a[i]->potential_distances = glm::vec4(real_min_distance, center_mass);
				}
				*/
			}
			
		}
		
	}

	//Define the semi-major and semi-minor axes of the ellipse (a and b).
	void generateEllipse(std::vector<glm::vec3>& line, float a, float b, int numPoints) {
		const float deltaTheta = 2.0f * glm::pi<float>() / numPoints;

		for (int i = 0; i < numPoints; ++i) {
			float theta = i * deltaTheta;
			float rho = std::sqrt(a * a * std::cos(theta) * std::cos(theta) + b * b * std::sin(theta) * std::sin(theta));
			glm::vec3 point(rho * std::cos(theta), rho * std::sin(theta), 0.0f);
			line.push_back(point);
		}
	}
	void midptellipse(int rx, int ry, int xc, int yc, std::vector<glm::vec3>& points)
	{
		float dx, dy, d1, d2, x, y;
		x = 0;
		y = ry;

		// Initial decision parameter of region 1
		d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
		dx = 2 * ry * ry * x;
		dy = 2 * rx * rx * y;

		// For region 1
		while (dx < dy)
		{
			// Add points based on 4-way symmetry
			points.push_back(glm::vec3(x + xc, y + yc, 0));
			points.push_back(glm::vec3(-x + xc, y + yc, 0));
			points.push_back(glm::vec3(x + xc, -y + yc, 0));
			points.push_back(glm::vec3(-x + xc, -y + yc, 0));

			// Update values based on algorithm
			if (d1 < 0)
			{
				x++;
				dx = dx + (2 * ry * ry);
				d1 = d1 + dx + (ry * ry);
			}
			else
			{
				x++;
				y--;
				dx = dx + (2 * ry * ry);
				dy = dy - (2 * rx * rx);
				d1 = d1 + dx - dy + (ry * ry);
			}
		}

		// Decision parameter of region 2
		d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);

		// Plotting points of region 2
		while (y >= 0)
		{
			// Add points based on 4-way symmetry
			points.push_back(glm::vec3(x + xc, y + yc, 0));
			points.push_back(glm::vec3(-x + xc, y + yc, 0));
			points.push_back(glm::vec3(x + xc, -y + yc, 0));
			points.push_back(glm::vec3(-x + xc, -y + yc, 0));

			// Update parameter value based on algorithm
			if (d2 > 0)
			{
				y--;
				dy = dy - (2 * rx * rx);
				d2 = d2 + (rx * rx) - dy;
			}
			else
			{
				y--;
				x++;
				dx = dx + (2 * ry * rx);
				dy = dy - (2 * rx * rx);
				d2 = d2 + dx - dy + (rx * rx);
			}
		}
	}

	glm::vec4 uniqueID()
	{
		if (ID[3] < 1)
		{
			ID[3] = ID[3] + 0.1f;
			nextColorID = ID;
			return nextColorID;
		}
		else if (nextColorID[2] < 1)
		{
			ID[2] = ID[2] + 0.1f;
			nextColorID = ID;
			return nextColorID;
		}
		else if (ID[1] < 1)
		{
			ID[1] = ID[1] + 0.1f;
			nextColorID = ID;
			return nextColorID;
		}
		else if (ID[0] < 1)
		{
			ID[0] = ID[0] + 0.1f;
			nextColorID = ID;
			return nextColorID;
		}
		else
		{
			std::cout << "Out of picking color";
			return glm::vec4(0,0,0,0);
		}
	}

	void UpdateProjectionLines()
	{
		if(Stereographicprojection)
		{ 
			projectionsline.clear();
		for (int i = 2; i < All_Meshs.size(); i++)
		{
			std::unique_ptr<mesh> line = std::make_unique<mesh>();
			std::vector<glm::vec3> Line;
			Line.push_back(All_Meshs[i]->transform.position);
			Line.push_back(projections[All_Meshs[i]->SafePointIndex]->transform.position);

			line->CreateLine(Line, glm::vec3(0.5, 0.6, 0.7));

			projectionsline.push_back(std::move(line));
		}
		}
	}
		
	std::vector<Vertex> Sphere_vert;
	std::vector<unsigned int> Sphere_ind;
	
	glm::vec3 RandomPosition();

	struct Edge {
		int vertex1;
		int vertex2;
		float distance;

		Edge(int v1, int v2, float dist) : vertex1(v1), vertex2(v2), distance(dist) {}
		bool operator<(const Edge& other) const {
			return distance < other.distance;
		}
	};
	

	int find(std::vector<int>& parent, int vertex) {
		if (parent[vertex] != vertex) {
			parent[vertex] = find(parent, parent[vertex]);
		}
		return parent[vertex];
	}


	void union_(std::vector<int>& parent, int vertex1, int vertex2) {
		parent[find(parent, vertex1)] = find(parent, vertex2);
	}
};
#endif
