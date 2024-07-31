#ifndef MESH_CLASS
#define MESH_CLASS
#include "Glew_Initialization.h"

class mesh
{
private:
	float sizeofline;
	float sizeofaxis;
	float pointsize = 5.0f;
	std::vector<Vertex> vertices;
	std::vector<Vertex> line_vertices;
	std::vector<unsigned int> indices;

	
	void Polygon_Mode(int index)
	{
		switch (index)
		{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPointSize(1);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(5);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
			
	}
	
	

	GLuint VAO, VBO, IBO;
	GLuint VAO_l, VBO_l, IBO_l;
	GLuint VAO_AXIS, VBO_AXIS, IBO_AXIS;

public:
	glm ::vec3 place_one_line()
	{
		current_place_one_line++;

		if (current_place_one_line > line_points.size())
		{

			current_place_one_line = -1;
			return transform.position-line_points[current_place_one_line];
		}
		else
		{
			return transform.position - line_points[current_place_one_line];
		}
		

	}
	int current_place_one_line = -1;
	std::vector<glm::vec3> line_points;
	glm::vec3 line_Color = glm::vec3(1,1,1);
	glm::vec3 Color = glm::vec3(1.0f, 0.1f, 1.0f);
	bool RelativeTobody = false;
	int GroupID = -1;
	int Lonely = 0;
	int currentIndex = 0;
	int SafePointIndex=0;
	std::vector<glm::vec3> ellipse;
	float equator_line;
	bool show_equator_line = true;
	Transforms transform;
	int Poly_mode = 0;
	glm::vec4 ColorID = glm::vec4(0, 0, 0,0);
	Plot_Data my_plot;
	std::vector < glm::vec3> grp_mates;

	glm::vec3 desired_position = glm::vec3(0, 0, 0);
	glm::vec4 potential_distances = glm::vec4(-1,0,0,0);

	bool move_on_elipse = false;
	bool traceMotion = false;
	int TraceInt = -1;
	mesh()
	{
		transform.Xdot.resize(6, 1);
		transform.Xdot << 0,
			0,
			0,
			0,
			0,
			0;
	}
	~mesh()
	{
		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}

		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}

		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO_l != 0)
		{
			glDeleteBuffers(1, &VBO_l);
			VBO_l = 0;
		}

		if (IBO_l != 0)
		{
			glDeleteBuffers(1, &IBO_l);
			IBO_l = 0;
		}

		if (VAO_l != 0)
		{
			glDeleteVertexArrays(1, &VAO_l);
			VAO_l = 0;
		}
		if(VBO_AXIS != 0)
		{
			glDeleteBuffers(1, &VBO_AXIS);
			VBO_AXIS = 0;
		}

		if (IBO_AXIS != 0)
		{
			glDeleteBuffers(1, &IBO_AXIS);
			IBO_AXIS = 0;
		}

		if (VAO_AXIS != 0)
		{
			glDeleteVertexArrays(1, &VAO_AXIS);
			VAO_AXIS = 0;
		}
	}
	void ClearMesh()
	{
		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}

		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}

		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO_l != 0)
		{
			glDeleteBuffers(1, &VBO_l);
			VBO_l = 0;
		}

		if (IBO_l != 0)
		{
			glDeleteBuffers(1, &IBO_l);
			IBO_l = 0;
		}

		if (VAO_l != 0)
		{
			glDeleteVertexArrays(1, &VAO_l);
			VAO_l = 0;
		}
		if (VBO_AXIS != 0)
		{
			glDeleteBuffers(1, &VBO_AXIS);
			VBO_AXIS = 0;
		}

		if (IBO_AXIS != 0)
		{
			glDeleteBuffers(1, &IBO_AXIS);
			IBO_AXIS = 0;
		}

		if (VAO_AXIS != 0)
		{
			glDeleteVertexArrays(1, &VAO_AXIS);
			VAO_AXIS = 0;
		}
	}
	void UpdateColor(glm::vec3 Color)
	{

		for (auto& Vertex : vertices)
		{
			Vertex.color = Color;
		}

		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}

		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}

		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}


	void CreatePoint()
	{

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void CreateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		this->indices = indices;
		this->vertices = vertices;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
	void RenderMesh(GLuint& shaderProgram, glm::mat4& CamMatrix)
	{
		
		glUseProgram(shaderProgram);

		Polygon_Mode(Poly_mode);
		
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
	
		Look_at(model);
		
		//model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		


		model = glm::scale(model, transform.scale);
		
		int outline = 0;
		glUniform1i(glGetUniformLocation(shaderProgram, "outline"), outline);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "CamMatrix"), 1, GL_FALSE, glm::value_ptr(CamMatrix));
		if (glGetUniformLocation(shaderProgram, "aColor") == 1)
		{
			glUniform4fv(glGetUniformLocation(shaderProgram, "aColor"), 1, glm::value_ptr(ColorID));
		}

		//normal shape
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		
		//outline
		if (Poly_mode == 0)
		{
			outline = 1;
			glUniform1i(glGetUniformLocation(shaderProgram, "outline"), outline);
			
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		
		

		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(0);

		
		
	}
	void CreateLine(std::vector<glm::vec3> LineLoc, glm::vec3 Color)
	{

		sizeofline = LineLoc.size();

		ellipse = LineLoc;

		line_vertices.clear();
		for (int i = 0; i < LineLoc.size(); i++)
		{
			line_vertices.push_back({ LineLoc[i], Color, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f) });


		}


		glGenVertexArrays(1, &VAO_l);
		glBindVertexArray(VAO_l);

		glGenBuffers(1, &VBO_l);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_l);
		glBufferData(GL_ARRAY_BUFFER, line_vertices.size() * sizeof(Vertex), line_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void CreateAxisLine(std::vector<glm::vec3> LineLoc, glm::vec3 Color)
	{

		sizeofaxis = LineLoc.size();

		ellipse = LineLoc;

		line_vertices.clear();
		for (int i = 0; i < LineLoc.size(); i++)
		{
			line_vertices.push_back({ LineLoc[i], Color, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f) });


		}


		glGenVertexArrays(1, &VAO_AXIS);
		glBindVertexArray(VAO_AXIS);

		glGenBuffers(1, &VBO_AXIS);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_AXIS);
		glBufferData(GL_ARRAY_BUFFER, line_vertices.size() * sizeof(Vertex), line_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DrawPoint(GLuint& shaderProgram, glm::mat4& CamMatrix)
	{

		glUseProgram(shaderProgram);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);



		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);

		//model = glm::translate(model, position);

		glPointSize(pointsize);

		// Perform the operation here
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "CamMatrix"), 1, GL_FALSE, glm::value_ptr(CamMatrix));


		glDrawArrays(GL_POINTS, 0, 1);


		glBindVertexArray(0);


		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	}
	void DrawLine(GLuint& shaderProgram, glm::mat4& CamMatrix)
	{

		if (VAO_l != 0)
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO_l);
		

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform.position);
			model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, transform.scale);
		
			// Perform the operation here
			int outline = 0;
			glUniform1i(glGetUniformLocation(shaderProgram, "outline"), outline);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "CamMatrix"), 1, GL_FALSE, glm::value_ptr(CamMatrix));

			glLineWidth(line_size);
			glDrawArrays(GL_LINE_STRIP, 0, sizeofline);
			glLineWidth(1);

			glBindVertexArray(0);


			glUseProgram(0);
		}
	}
	void DrawAxis(GLuint& shaderProgram, glm::mat4& CamMatrix)
	{

		if (VAO_AXIS != 0)
		{
			glUseProgram(shaderProgram);
			
			glBindVertexArray(VAO_AXIS);



			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform.position);
			model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, transform.scale);

			// Perform the operation here
			int outline = 0;
			glUniform1i(glGetUniformLocation(shaderProgram, "outline"), outline);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "CamMatrix"), 1, GL_FALSE, glm::value_ptr(CamMatrix));

			glLineWidth(2);
			glDrawArrays(GL_LINE_STRIP, 0, sizeofaxis);
			glLineWidth(1);

			glBindVertexArray(0);


			glUseProgram(0);
			
		}
	}
	void gen_line()
	{
		move_on_elipse = false;
		line_points.clear();
		current_place_one_line = -1;
		int numPoints = 5760;
		const float deltaTheta = 2.0f * glm::pi<float>() / numPoints;
		float a = 30;
		float b = 30;
		for (int i = 0; i < numPoints; ++i) {
			float theta = i * deltaTheta;
			float rho = std::sqrt(a * a * std::cos(theta) * std::cos(theta) + b * b * std::sin(theta) * std::sin(theta));
			glm::vec3 point(rho * std::cos(theta), 0.0f, rho * std::sin(theta));
			line_points.push_back(point);
		}
		line_points.push_back(line_points[0]);
		CreateLine(line_points, glm::vec3(1, 1, 0));
		//line_points.clear();
		//line_points.push_back(glm::vec3(0,-0.4 ,0));
		//line_points.push_back(glm::vec3(0, 0.4,0));
		//CreateAxisLine(line_points, glm::vec3(1, 1, 0));
	}
	void Look_at(glm::mat4 &model)
	{
		if (glm::length(transform.velocity) > 0.0f) {
			
			glm::vec3 direction = glm::normalize(transform.velocity); // Normalized direction of velocity
			glm::vec3 initialForward = glm::vec3(1.0f, 0.0f, 0.0f);
			// Compute the target direction (here, assuming you want to look ahead along the velocity direction)
			glm::vec3 targetDirection = direction;

			glm::quat rotationQuat = glm::rotation(initialForward, direction);

			glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
			model = model * rotationMatrix;
		}
	}
	float FindRot_y(const float x, const float y, const float z)
	{
		const float r = std::sqrt(x * x + z * z);
		if (r == 0)
		{
			return 0;
		}
		const float temp_theta = std::asin(-z / r) * 180 / std::acos(-1);
	
		if (x < 0)
		{
			return 180 - temp_theta;
		}
		return temp_theta;
	}
	float FindRot_z(const float x, const float y, const float z)
	{
		const float r = std::sqrt(x * x + y * y + z * z);
		const float temp_theta = std::asin(y / r) * 180/ std::acos(-1);
		if (x < 0)
		{
			return 180 - temp_theta;
		}
		return temp_theta;
	}
};
#endif
