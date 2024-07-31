#ifndef LINE_CLASS
#define LINE_CLASS
#include "Glew_Initialization.h"

class lineT
{
private:
	float sizeofline;
	float pointsize = 5.0f;
	std::vector<Vertex> vertices;
	std::vector<Vertex> line_vertices;
	std::vector<unsigned int> indices;


	std::vector<glm::vec3> line_points;

	GLuint VAO_l, VBO_l, IBO_l;


public:
	glm::vec3 line_Color = glm::vec3(1, 1, 1);
	glm::vec3 Color = glm::vec3(1.0f, 0.1f, 1.0f);


	Transforms transform;

	bool ShowLine = false;

	lineT()
	{
		transform.Xdot.resize(6, 1);
		transform.Xdot << 0,
			0,
			0,
			0,
			0,
			0;
	}
	~lineT()
	{
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
		
	}
	void ClearMesh()
	{
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
		
	}


	void UpdateLine(glm::vec3 LineLoc)
	{
		glm::vec3 Color = glm::vec3(1, 0, 0.5);
		line_vertices.push_back({ LineLoc, Color, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f) });
		sizeofline = line_vertices.size();

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



	
	void CreateLine(std::vector<glm::vec3> LineLoc, glm::vec3 Color)
	{

		sizeofline = LineLoc.size();


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

			glLineWidth(2);
			glDrawArrays(GL_LINE_STRIP, 0, sizeofline);
			glLineWidth(1);

			glBindVertexArray(0);


			glUseProgram(0);
		}
	}
	
};
#endif
