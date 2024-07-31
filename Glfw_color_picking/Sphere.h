#ifndef SPHERE_CLASS
#define SPHERE_CLASS
#include "Glew_Initialization.h"

class GenSphere
{
	public:
		void generateSphere(float radius, unsigned int rings, unsigned int sectors, glm::vec3 color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
			float const R = 1.0f / (float)(rings - 1);
			float const S = 1.0f / (float)(sectors - 1);

			vertices.resize(rings * sectors);
			std::vector<Vertex>::iterator v = vertices.begin();

			for (unsigned int r = 0; r < rings; ++r) {
				for (unsigned int s = 0; s < sectors; ++s) {
					float const y = sin(-glm::pi<float>() / 2 + glm::pi<float>() * r * R);
					float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
					float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

					v->position = glm::vec3(x, y, z) * radius;
					v->color = color;
					v->texUV = glm::vec2(s * S, r * R);
					v++;
				}
			}

			indices.resize(rings * sectors * 6);
			std::vector<unsigned int>::iterator i = indices.begin();
			for (unsigned int r = 0; r < rings - 1; ++r) {
				for (unsigned int s = 0; s < sectors - 1; ++s) {
					*i++ = r * sectors + s;
					*i++ = r * sectors + (s + 1);
					*i++ = (r + 1) * sectors + (s + 1);
					*i++ = (r + 1) * sectors + s;
					*i++ = r * sectors + s;
					*i++ = (r + 1) * sectors + (s + 1);
				}
			}
		}
		void generateBox(glm::vec3 color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
		{
			vertices =
			{
				{glm::vec3(-1,-1,1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//0
				{glm::vec3(1,-1,1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//1
				{glm::vec3(-1,1,1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//2
				{glm::vec3(1,1,1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//3
				{glm::vec3(-1,-1,-1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//4
				{glm::vec3(1,-1,-1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//5
				{glm::vec3(-1,1,-1),color,glm::vec3(0,0,0),glm::vec2(0,0)},//6
				{glm::vec3(1,1,-1),color,glm::vec3(0,0,0),glm::vec2(0,0)}//7
			};
			indices =
			{
				0,1,0,
				1,3,1,
				3,2,3,
				2,0,2,
				1,5,1,
				5,7,5,
				7,3,7,
				7,6,7,
				5,4,5,
				4,6,4,
				0,4,0,
				6,2,6

				/*0,1,3,
				0,3,2,
				1,5,7,
				1,7,3,
				0,4,6,
				0,6,2,
				4,5,7,
				4,7,6*/
			};


		}
		void generateTriangle(glm::vec3 color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
		{
			vertices =
			{
				{glm::vec3(1.3,0,0),color,glm::vec3(0,0,0),glm::vec2(0,0)},//0
				{glm::vec3(0,0.5,0),color,glm::vec3(0,0,0),glm::vec2(0,0)},//1
				{glm::vec3(0,-0.2,0.3),color,glm::vec3(0,0,0),glm::vec2(0,0)},//2
				{glm::vec3(0,-0.2,-0.3),color,glm::vec3(0,0,0),glm::vec2(0,0)}//3
				

				
				
			};
			indices =
			{
				0,1,2,
				0,3,2,
				0,1,3,
				3,1,2
			};


		}
		void generatePlane(int i,glm::vec3 color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
		{
			vertices =
			{
				{glm::vec3(-i,0,-i),color,glm::vec3(0,0,0),glm::vec2(0,0)},//0
				{glm::vec3(-i,0,i),color,glm::vec3(0,0,0),glm::vec2(0,0)},//1
				{glm::vec3(i,0,i),color,glm::vec3(0,0,0),glm::vec2(0,0)},//2
				{glm::vec3(i,0,-i),color,glm::vec3(0,0,0),glm::vec2(0,0)}//3

			};
			indices =
			{
				0,1,2,
				2,3,0
			};


		}
};
#endif