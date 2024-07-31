#include "Glew_Initialization.h"
GLFWwindow* Main_Window = nullptr;
glm::vec3 Camera_Position = glm::vec3(0, 0, 50.0f);
int SwarmSize = 2;
float min_drift = 5.0f;
float  min_distance = 11.0f;
float real_min_distance = 10.0f;
bool Connecting_Line = true;
glm::vec4 Background_Color = glm::vec4(0.2, 0.2, 0.2, 0.2);
float G_const = 6.67430e-11;
float dt = 0.06f;
float avg_raduis =0.3f;
float currentTime = 0;
float line_size = 2;
double wt = 0.0;
bool ShowScript = false;
double omega = 0;
double c_mass = 1;
Eigen::MatrixXd Gain = (Eigen::MatrixXd(3, 6) << 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0).finished();

bool exclude_drift = false;
bool ApplyLqr = false;
bool ApplyPotential = true;
bool ApplyCLW = true;
bool ApplyPotential_To_One = false;
float Ca = 4.5;
float Cr = 4;
float la = 5;
float lr = 2.755;
float fr = 0.5;
float E = 1.5;
float alpha = 0.3;
bool can_read_Overlay = true;

 std::vector <float>All_Vector_Sum = {0};
 std::vector <float>Fuel_Consumption = { 0 };
 std::vector <float>Fuel_Time = { 0 };
 std::vector<float> All_vector_time = { 0 };
 glm::vec3 boundary_limits = glm::vec3(0, 0, 0);
 float damping = 1.0f;
 bool use_center_mass = false;
 bool Rotate_to_velocity = true;
 float min_space_bound_check = FLT_MAX;
 float cur_space_bound_check = 0;
 float max_space_bound_check = FLT_MIN;
 float collision_raduis = 3;
 bool add_repulsive_potential = true;
 float collide_number = 0;
 bool limit_velocity = false;
 bool potential_center = true;
 bool Stereographicprojection = false;
 bool debug_sphere = false;
 bool UseRandomValueFromFile = true;
