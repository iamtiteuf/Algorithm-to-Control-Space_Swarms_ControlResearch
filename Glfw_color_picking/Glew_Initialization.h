#ifndef ENVIR_INITIALIZE_H
#define ENVIR_INITIALIZE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <memory>
#include <Eigen/Dense>

extern std::vector <float>All_Vector_Sum;
extern std::vector<float> All_vector_time;
extern std::vector <float>Fuel_Consumption;
extern std::vector <float>Fuel_Time;
extern glm::vec3 boundary_limits;
extern float damping;
extern float G_const;
extern glm::vec4 Background_Color;
extern glm::vec3 Camera_Position;
extern int SwarmSize;
extern float min_drift;
extern float min_distance;
extern float real_min_distance;
extern float dt;
extern float avg_raduis;
extern enum Linetype;
extern double wt;
extern bool ShowScript, exclude_drift;
extern Eigen::MatrixXd Gain;
extern double omega;
extern double c_mass;
extern bool ApplyLqr,ApplyPotential,ApplyCLW,ApplyPotential_To_One;
extern float Ca, Cr, la,E ,lr , fr, alpha,collide_number, currentTime ;
extern bool can_read_Overlay;
extern bool Connecting_Line;
extern bool Rotate_to_velocity, use_center_mass;
extern 	enum All_Potential_Type;
extern float min_space_bound_check;
extern float cur_space_bound_check;
extern float max_space_bound_check;
extern float collision_raduis;
extern bool add_repulsive_potential,limit_velocity, Stereographicprojection,debug_sphere;
extern float line_size;
extern bool potential_center;
extern bool UseRandomValueFromFile;
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texUV;
};

struct Transforms
{
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::quat n_rotaion = glm::quat(0, 0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	float mass = 10.0f;
	float gravity = 10.0f;
	float raduis = 1.0f;
	float drift = 0.0f;
	Eigen::MatrixXd Xdot;
};

extern GLFWwindow* Main_Window;

enum All_Potential_Type
{
	Complex = 0,
	Harmonic_Potential = 1,
	Lennard_Jones_Potential = 2,
	Mei_Potential = 3,
	virtual_Potential = 4
};
enum Linetype
{
	Original = 0,
	Dijkstra = 1,
	Kruskals = 2
};
struct Plot_Data
{
	std::vector<float>position_x;
	std::vector<float>position_y;
	std::vector<float>position_z;
	std::vector<float> velocity_x;
	std::vector<float> velocity_y;
	std::vector<float> velocity_z;
	std::vector<float> time;
	std::vector<float> position;
	std::vector<float> velocit;
	
	std::vector<float> integral_norm;
};

class Glew_Initialization
{
public:

	
	float width = 1920;
	float height = 1080;
	int Init_Glfw()
	{
		if (!glfwInit())
		{
			return EXIT_FAILURE;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		Main_Window = glfwCreateWindow(width, height, "ControlResearch", NULL, NULL);
		
		if (!Main_Window )
		{
			glfwTerminate();
			return EXIT_FAILURE;
		}
		glfwMakeContextCurrent(Main_Window );
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			glfwDestroyWindow(Main_Window );
			glfwTerminate();

			std::cout << "failed to initialize glew";
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

};


#endif
