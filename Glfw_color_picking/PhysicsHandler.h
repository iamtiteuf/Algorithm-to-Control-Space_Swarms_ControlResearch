#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H
#include "ScriptingArea.h"
#include "Glew_Initialization.h"
#include "PID.h"
#include "ControlScript.h"
#include "logger.h"
class PhysicsHandler
{
	 
	public:	
		
		static void LQR_Solver(Eigen::MatrixXd& K,float n, float m,int iter)
		{
			K = LQR(n, m, 1);

			if (K(0, 0) == INFINITY || isnan(K(0, 0)))
			{
				K = LQR(n, m, powf(10, iter));

			}
			/*for (int i = iter; i>= -iter; i--)
			{
				if (K(0, 0) == INFINITY || isnan(K(0, 0)))
				{
					K = LQR(n, m, powf(10,i));
					
				}
				else
				{
					break;
				}
		
			}*/
			if (K(0, 0) == INFINITY || isnan(K(0, 0)))
			{
				K.setIdentity();
			}

			//std::cout << " K " << "\n" << K << "\n" << "\n";
		}
	
		static Eigen::MatrixXd LQR(float n, float m,float scl)
		{
			Eigen::MatrixXd A;
			A.resize(6, 6);
			A << 0, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 1,
				3 * n * n, 0, 0, 0, 2 * n, 0,
				0, 0, 0, -2 * n, 0, 0,
				0, 0, -n * n, 0, 0, 0;
			/*A << 0, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 1,
				3 * n * n, 0, 0, 0, 2 * n, 0,
				0, 0, 0, -2 * n, 0, 0,
				0, 0, -n * n, 0, 0, 0;*/
			Eigen::MatrixXd B;
			B.resize(6, 3);
			B << 0, 0, 0, 
				 0, 0,0,
				 0, 0, 0, 
				 1 / m, 0, 0,
				 0, 1 / m, 0,
				 0, 0, 1 / m;

			Eigen::MatrixXd Q;
			Q.resize(6, 6);
			Q << 1, 0, 0, 0, 0, 0,
				 0, 1, 0, 0, 0, 0,
				 0, 0, 1, 0, 0, 0,
				 0, 0, 0, 1, 0, 0,
				 0, 0, 0, 0, 1, 0,
				 0, 0, 0, 0, 0, 1;

			Q = Q * scl;

			Eigen::MatrixXd R;
			R.resize(3, 3);
			R << 1, 0, 0,
				 0, 1, 0,
				 0, 0, 1;

			Eigen::MatrixXd HM;
			HM.resize(2 * A.rows(), 2 * A.cols());

			
			
			HM << A, -B * R.inverse() * B.transpose(),
				-Q, -A.transpose();

			
			Eigen::EigenSolver<Eigen::MatrixXd> solver(HM);
			Eigen::MatrixXd eigenvectors = solver.eigenvectors().real();
			Eigen::VectorXcd eigenvalues = solver.eigenvalues();

			// Create a vector of pairs, where each pair contains an eigenvalue and its corresponding eigenvector
			std::vector<std::pair<std::complex<double>, Eigen::VectorXd>> eigenPairs;
			for (int i = 0; i < eigenvalues.size(); ++i)
			{
				if (eigenvalues[i].real() < 0)
				{
					eigenPairs.push_back(std::make_pair(eigenvalues[i], eigenvectors.col(i).real()));
				}
			}

			// Sort the vector of pairs based on the eigenvalues
			std::sort(eigenPairs.begin(), eigenPairs.end(),
				[](const std::pair<std::complex<double>, Eigen::VectorXd>& a,
					const std::pair<std::complex<double>, Eigen::VectorXd>& b)
				{
					return a.first.real() < b.first.real();
				});

			// Create a matrix of eigenvectors sorted according to their eigenvalues
			Eigen::MatrixXd sortedEigenvectors(eigenvectors.rows(), eigenPairs.size());
			for (int i = 0; i < eigenPairs.size(); ++i)
			{
				sortedEigenvectors.col(i) = eigenPairs[i].second;
			}

		

			//Eigen::MatrixXd eigenvectorsMatrix(eigenvectors.rows(), 0); // Initialize an empty matrix

			//for (int i = 0; i < eigenvalues.size(); ++i) 
			//{
			//	if (eigenvalues[i].real() < 0) { // If the real part of the eigenvalue is negative
			//		eigenvectorsMatrix.conservativeResize(Eigen::NoChange, eigenvectorsMatrix.cols() + 1); // Resize the matrix to add a new column
			//		eigenvectorsMatrix.col(eigenvectorsMatrix.cols() - 1) = eigenvectors.col(i); // Add the eigenvector to the matrix
			//	}
			//}

			//std::cout << "Eigenvectors matrix:\n" << eigenvectorsMatrix << std::endl;

			Eigen::MatrixXd X;
			Eigen::MatrixXd Y;

			int halfRows = sortedEigenvectors.rows() / 2;

			X = sortedEigenvectors.block(0, 0, halfRows, sortedEigenvectors.cols());
			Y = sortedEigenvectors.block(halfRows, 0, halfRows, sortedEigenvectors.cols());
			
			if (X.rows() == Y.cols())
			{
				Eigen::MatrixXd P = X * Y.inverse();
				Eigen::MatrixXd K = -R.inverse() * B.transpose() * P;
				return K;
			}
			else
			{
				Eigen::MatrixXd P;
				P.resize(6, 6);
				P.setConstant(NAN);
				return P;
			}
			
			/*std::cout << "X dimensions: " << X.rows() << "x" << X.cols() << std::endl;
			std::cout << "Y dimensions: " << Y.rows() << "x" << Y.cols() << std::endl << "\n";
			std::cout << eigenvectorsMatrix <<"\n" << "\n";


			std::cout << "X " << X << "\n" << "\n";
			std::cout << "Y " << Y << "\n" << "\n";
			std::cout << eigenvalues << "\n" << "\n";
			std::cout << "Y-1 " << Y.inverse() << "\n" << "\n";
			std::cout << P << "\n" << "\n";*/
			
		}

		
		
		static void Step(Transforms & transform)
		{
			transform.acceleration = transform.force / transform.mass * dt;
			transform.velocity += transform.acceleration * dt;
			transform.position += transform.velocity * dt;
			transform.force = glm::vec3(0, 0, 0);
			Boundary(transform);
		}
		static void Calc_Omega(mesh* obj1, mesh* target)
		{
			glm::vec3 r = obj1->transform.position - target->transform.position;
			glm::vec3 v = obj1->transform.velocity - target->transform.velocity;

			//target->transform.velocity = glm::vec3(0, 0, 0);

			omega = sqrt(G_const * (obj1->transform.mass + target->transform.mass) / powf(glm::length(r), 3));
			c_mass = obj1->transform.mass;
		}
		static void Forces(std::vector<mesh*> &All_Mesh,Script_Editor& pyIntergrate,std::vector<float> & t_data, std::vector<float>& x_data, std::vector<float>& y_data, std::vector<float>& z_data, PID& pid,All_Potential_Type potential_type, ControlScript &Control,Log &dispersion,Log &sat1, Log& sat2, Log& sat3, Log& sat4)
		{
			
			float targetmass = All_Mesh[1]->transform.mass;
			
			wt = wt + dt;
			for (int i = 0; i < All_Mesh.size(); i++)
			{
				if (i == 0 && Control.eliptic_orbit->move_on_elipse)
				{
					All_Mesh[1]->transform.position = Control.eliptic_orbit->place_one_line();
				}
				
				if (i >= 2)
				{
					
					glm::vec3 v_desired = glm::vec3(.0001f, .0001f, .0001f);

					glm::vec3 r = All_Mesh[i]->transform.position - All_Mesh[1]->transform.position;
					glm::vec3 v = All_Mesh[i]->transform.velocity - All_Mesh[1]->transform.velocity;

					Calc_Omega(All_Mesh[i], All_Mesh[1]);

					if (ApplyLqr)
					{
						pyIntergrate.RunScript(omega);
					}

					if (ApplyCLW)
					{
						CLW(All_Mesh[i], r, v);
					}
					

					//All_Mesh[i]->transform.drift = All_Mesh[i]->transform.velocity.y + 2.0f * All_Mesh[i]->transform.position.x;
					if (potential_center)
					{
						All_Mesh[i]->potential_distances.x = 1;
					}
					if (ApplyPotential)
					{
						
						//All_Mesh[i]->transform.force.x += Potential_Force_e(r, v, All_Mesh[i]->transform.position.x, 0);
						//All_Mesh[i]->transform.force.y += Potential_Force_e(r, v, All_Mesh[i]->transform.position.y, 1);
						//All_Mesh[i]->transform.force.z += Potential_Force_e(r, v, All_Mesh[i]->transform.position.z, 2);
						if(All_Mesh[i]->potential_distances.x>0)
						{ 
							switch (potential_type)
							{
							case 0:
								All_Mesh[i]->transform.force.x += Potential_Force_e(r, v, All_Mesh[i]->transform.position.x, 0);
								All_Mesh[i]->transform.force.y += Potential_Force_e(r, v, All_Mesh[i]->transform.position.y, 1);
								All_Mesh[i]->transform.force.z += Potential_Force_e(r, v, All_Mesh[i]->transform.position.z, 2);
								break;
							case 1:
								glm::vec3 other;
								
								if (potential_center && !Stereographicprojection)
								{
									other = All_Mesh[1]->transform.position;
									All_Mesh[i]->transform.force.x += Harmonic_Potential(All_Mesh[i]->transform.position, other, 0, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
									All_Mesh[i]->transform.force.y += Harmonic_Potential(All_Mesh[i]->transform.position, other, 1, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
									All_Mesh[i]->transform.force.z += Harmonic_Potential(All_Mesh[i]->transform.position, other, 2, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
								}
								else if (Stereographicprojection)
								{
									std::cout << "tf?";
									other = Control.projections[All_Mesh[i]->SafePointIndex]->transform.position;
									All_Mesh[i]->transform.force.x += Harmonic_Potential(All_Mesh[i]->transform.position, other, 0, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
									All_Mesh[i]->transform.force.y += Harmonic_Potential(All_Mesh[i]->transform.position, other, 1, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
									All_Mesh[i]->transform.force.z += Harmonic_Potential(All_Mesh[i]->transform.position, other, 2, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
								}else
								{
									//other = glm::vec3(All_Mesh[i]->potential_distances.y, All_Mesh[i]->potential_distances.z, All_Mesh[i]->potential_distances.w);
									for (int j = 0; j < All_Mesh[i]->grp_mates.size(); j++)
									{
										
										other = All_Mesh[i]->grp_mates[j];
										//All_Mesh[i]->potential_distances.x = real_min_distance;
										All_Mesh[i]->transform.force.x += Harmonic_Potential(All_Mesh[i]->transform.position, other, 0, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
										All_Mesh[i]->transform.force.y += Harmonic_Potential(All_Mesh[i]->transform.position, other, 1, r, v, real_min_distance, All_Mesh[i]->transform.velocity);
										All_Mesh[i]->transform.force.z += Harmonic_Potential(All_Mesh[i]->transform.position, other, 2, r, v, real_min_distance, All_Mesh[i]->transform.velocity);

									}
								}
								
							
								break;
							case 2:
								All_Mesh[i]->transform.force.x += Lennard_Jones_Potential(All_Mesh[i]->transform.position, 0, r, v);
								All_Mesh[i]->transform.force.y += Lennard_Jones_Potential(All_Mesh[i]->transform.position, 1, r, v);
								All_Mesh[i]->transform.force.z += Lennard_Jones_Potential(All_Mesh[i]->transform.position, 2, r, v);
								break;
							case 3:
								All_Mesh[i]->transform.force.x += Mei_Potential(All_Mesh[i]->transform.position, 0, r, v);
								All_Mesh[i]->transform.force.y += Mei_Potential(All_Mesh[i]->transform.position, 1, r, v);
								All_Mesh[i]->transform.force.z += Mei_Potential(All_Mesh[i]->transform.position, 2, r, v);
								break;
							case 4:
								glm::vec3 other_2;
								if (potential_center && !Stereographicprojection)
								{
									other_2 = All_Mesh[1]->transform.position;

								}
								else if (Stereographicprojection)
								{

									other_2 = Control.projections[All_Mesh[i]->SafePointIndex]->transform.position;
								}
								else
								{
									other_2 = glm::vec3(All_Mesh[i]->potential_distances.y, All_Mesh[i]->potential_distances.z, All_Mesh[i]->potential_distances.w);
								}
								
								//All_Mesh[i]->potential_distances.x = real_min_distance;
								All_Mesh[i]->transform.force.x += Virtual_potential(All_Mesh[i]->transform.position, other_2, 0, r, v, All_Mesh[i]->potential_distances.x);
								All_Mesh[i]->transform.force.y += Virtual_potential(All_Mesh[i]->transform.position, other_2, 1, r, v, All_Mesh[i]->potential_distances.x);
								All_Mesh[i]->transform.force.z += Virtual_potential(All_Mesh[i]->transform.position, other_2, 2, r, v, All_Mesh[i]->potential_distances.x);
								break;
							}

							/*if (add_repulsive_potential)
							{
								float dis = 0;
								for (int j = 0; j < All_Mesh.size(); j++)
								{
									if (i != j)
									{
										dis = glm::distance(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position);
										if (dis < collision_raduis)
										{
											All_Mesh[i]->transform.force += repulsive_potential(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position);
											if (i == 3)
											{
												std::cout << repulsive_potential(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position).x << "\n";
											}
										}

									}
								}
								
							}*/
						}
						

					}

					if (add_repulsive_potential)
					{
						float dis = 0;
						for (int j =2; j < All_Mesh.size(); j++)
						{
							if (i != j)
							{
								dis = glm::distance(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position);
								if (dis < collision_raduis)
								{
									All_Mesh[i]->transform.force += repulsive_potential(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position);
							
								}
								
							}
						}

					}
					
					//PID_Control(glm::vec3(0, 06, 0), All_Mesh[i]->transform.position, pid);

					if (ApplyLqr)
					{
						Lqr(All_Mesh[i], All_Mesh[1]);
					}

				}
				
				
				All_Mesh[i]->my_plot.time.push_back(wt);

				All_Mesh[i]->my_plot.position_x.push_back(All_Mesh[i]->transform.position.x);
				All_Mesh[i]->my_plot.position_y.push_back(All_Mesh[i]->transform.position.y);
				All_Mesh[i]->my_plot.position_z.push_back(All_Mesh[i]->transform.position.z);

				All_Mesh[i]->my_plot.velocity_x.push_back(All_Mesh[i]->transform.velocity.x);
				All_Mesh[i]->my_plot.velocity_y.push_back(All_Mesh[i]->transform.velocity.y);
				All_Mesh[i]->my_plot.velocity_z.push_back(All_Mesh[i]->transform.velocity.z);
				
				All_Mesh[i]->my_plot.position.push_back(glm::length(All_Mesh[i]->transform.position));

				if (i == 5)
				{
					std::string time = std::to_string(wt);
					std::string value = std::to_string(glm::length(All_Mesh[i]->transform.position));;
					sat1.logMessage(time, value);
				}
				if (i == 3)
				{
					std::string time = std::to_string(wt);
					std::string value = std::to_string(glm::length(All_Mesh[i]->transform.position));;
					sat2.logMessage(time, value);
				}
				if (i == 7)
				{
					std::string time = std::to_string(wt);
					std::string value = std::to_string(glm::length(All_Mesh[i]->transform.position));;
					sat3.logMessage(time, value);
				}
				if (i == 11)
				{
					std::string time = std::to_string(wt);
					std::string value = std::to_string(glm::length(All_Mesh[i]->transform.position));;
					sat4.logMessage(time, value);
				}

				//All_Mesh[i]->my_plot.integral_norm.push_back(norm);

				//x_data.push_back(All_Mesh[i]->transform.Xdot(0,0));//position x y and z
				//y_data.push_back(All_Mesh[i]->transform.Xdot(1, 0));
				//z_data.push_back(All_Mesh[i]->transform.Xdot(2, 0));
				
			}
			
			Sum_Contained(All_Mesh, dispersion);
		}
		
		static float Potential_Force_e(glm::vec3 rv, glm::vec3 rvdot, float position, int i)
		{
				float r = glm::length(rv);
				float er = glm::dot(rv, rvdot);
				glm::vec3 v = rvdot - rv * er / glm::length(rv);	
				glm::vec3 vtd = alpha * v / glm::length(v);
				float friction = (fr * er * position / glm::length(rv));
			
				return ((-Ca / la) * exp(-r / la) + (Cr / lr) * exp(-r / lr)) * (position / r) - friction - fr * (v[i] - vtd[i]);
						
		}
		static void CLW(mesh* &obj,glm::vec3 r, glm::vec3 v)
		{
			obj->transform.force.x = 3 * omega * omega * r.x + 2 * omega * v.y;
			obj->transform.force.y = -2 * omega * v.x;
			obj->transform.force.z = -omega * omega * r.z;
		}
		static void Lqr(mesh* &obj, mesh*& target)
		{
			Eigen::MatrixXd K;
			K = Gain;
			Eigen::MatrixXd U;
			Eigen::MatrixXd X;
			X.resize(6, 1);
			X << obj->transform.position.x,
				obj->transform.position.y,
				obj->transform.position.z,
				obj->transform.velocity.x,
				obj->transform.velocity.y,
				obj->transform.velocity.z;

			Eigen::MatrixXd Xdesiredstate;
			Xdesiredstate.resize(6, 1);

			Xdesiredstate << target->transform.position.x,
				target->transform.position.y,
				target->transform.position.z,
				target->transform.velocity.x,
				target->transform.velocity.y,
				target->transform.velocity.z;;

			Eigen::MatrixXd A;
			A.resize(6, 6);
			A << 0, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 1,
				3 * omega * omega, 0, 0, 0, 2 * omega, 0,
				0, 0, 0, -2 * omega, 0, 0,
				0, 0, -omega * omega, 0, 0, 0;

			Eigen::MatrixXd B;
			B.resize(6, 3);
			B << 0, 0, 0,
				0, 0, 0,
				0, 0, 0,
				1 / obj->transform.mass, 0, 0,
				0, 1 / obj->transform.mass, 0,
				0, 0, 1 / obj->transform.mass;

			U = -K * (X - Xdesiredstate);
			Eigen::MatrixXd X_next = X + dt * (A * X + B * U);
			obj->transform.Xdot = X + dt * (A * X + B * U);

			obj->transform.position.x = X_next(0, 0);
			obj->transform.position.y = X_next(1, 0);
			obj->transform.position.z = X_next(2, 0);

			obj->transform.velocity.x = X_next(3, 0);
			obj->transform.velocity.y = X_next(4, 0);
			obj->transform.velocity.z = X_next(5, 0);
		}

		static void PID_Control(glm::vec3 Target_position, glm::vec3& Object_position,PID& pid)
		{
			pid.PID_Controller(Target_position,Object_position);
		}
		static void Target_Orbit(mesh* obj,std::vector<glm::vec3>line)
		{
			// Define the speed of the orbit
			const float speed = 1.0f;

			// Define the radius of the orbit
			const float radius = 10.0f;

			// Increment the angle based on the speed and elapsed time
			static float angle = 0.0f;
			angle += speed * dt;

			// Calculate the new position of the object
			obj->transform.position.x = radius * cos(angle);
			obj->transform.position.y = radius * sin(angle);
			obj->transform.position.z = 0.0f; // Keep the object on the same plane
		}
		static float Harmonic_Potential(glm::vec3 pos, glm::vec3 pos_2,float i, glm::vec3 rv, glm::vec3 rvdot,float rd,glm::vec3 velocityi)
		{
			
			//float r = sqrt(powf((pos.x - pos_2.x),2)+ powf((pos.y - pos_2.y), 2)+ powf((pos.z - pos_2.z), 2));
			float r = glm::distance(pos, pos_2);
			float er = glm::dot(rv, rvdot);
			glm::vec3 v = rvdot - rv * (er / glm::length(rv));
			glm::vec3 vtd = alpha * v / glm::length(v);
			float friction = 0.0f;
			if (limit_velocity)
			{
				if (glm::length(velocityi) > la)
				{
					friction = ((fr * lr) * er * pos[i] / glm::length(rv));
					return -pos[i] * (1.0f - rd / r) * Ca - friction - (fr * lr) * (v[i] - vtd[i]);
				}
				else
				{
					friction = (fr * er * pos[i] / glm::length(rv));
					return -pos[i] * (1.0f - rd / r) * Ca - friction - fr * (v[i] - vtd[i]);
				}
				
			}
			else
			{
				friction = (fr * er * pos[i] / glm::length(rv));
				//std::cout << i << "\t" << -pos[i] * (1 - rd / r) * Ca - friction - fr * (v[i] - vtd[i]) << "\n";
				return -pos[i] * (1.0f - rd / r) * Ca - friction - fr * (v[i] - vtd[i]);
			}
			
		}
		static float Virtual_potential(glm::vec3 pos, glm::vec3 pos_2, float i, glm::vec3 rv, glm::vec3 rvdot, float rd)
		{
			float r = sqrt(powf((pos.x - pos_2.x), 2) + powf((pos.y - pos_2.y), 2) + powf((pos.z - pos_2.z), 2));
			float er = glm::dot(rv, rvdot);
			glm::vec3 v = rvdot - rv * er / glm::length(rv);
			glm::vec3 vtd = alpha * v / glm::length(v);

			float friction = (fr * er * pos[i] / glm::length(rv));

			//repulsive
			float partial_deriv = ((pos.x - pos_2.x) / r) + ((pos.y - pos_2.y) / r)+ ((pos.z - pos_2.z) / r);

			float repulsize = 0;
			/*float dis = glm::distance(pos, pos_2);
			if (dis < real_min_distance)
			{
				repulsize = Ca / (dis * dis * dis)* partial_deriv;
			}
			else
			{
				repulsize = 0;
			}*/
			
			return Ca * (pos_2[i]-pos[i]) - friction - fr * (v[i] - vtd[i]) + repulsize;
		}
		static glm::vec3 repulsive_potential(glm::vec3 pos, glm::vec3 pos_2)
		{
			float r = sqrt(powf((pos.x - pos_2.x), 2) + powf((pos.y - pos_2.y), 2) + powf((pos.z - pos_2.z), 2));
			
			float par_deriv_x = ((pos.x - pos_2.x) / r);
			float par_deriv_y = ((pos.y - pos_2.y) / r);
			float par_deriv_z = ((pos.z - pos_2.z) / r);
			glm::vec3 repulsize;
			//float dis = glm::distance(pos, pos_2);
			float dis = collision_raduis;
			if (dis != 0)
			{
				repulsize = (Cr / (dis * dis * dis)) * glm::vec3(par_deriv_x, par_deriv_y, par_deriv_z);
			}
			else
			{
				repulsize = glm::vec3(0,0,0);
			}
			//std::cout << "Force : " << repulsize.x << "\t" << repulsize.y << "\t" << repulsize.z << "\n";
			return repulsize;
		}
		
		static float Lennard_Jones_Potential(glm::vec3 pos, float i, glm::vec3 rv, glm::vec3 rvdot)
		{
			float rd = 0.7f;
			float A = 4 * powf(rd, 12) * E;
			float B = 4 * E * powf(rd, 6);

			float er = glm::dot(rv, rvdot);
			glm::vec3 v = rvdot - rv * er / glm::length(rv);
			glm::vec3 vtd = alpha * v / glm::length(v);
			float friction = (fr * er * pos[i] / glm::length(rv));

			float ans = -((-12 * pos[i] * A) / powf((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z), 7) + (6 * B * pos[i]) / powf((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z), 4));
			return ans - friction - fr * (v[i] - vtd[i]);
		}
		static float Mei_Potential(glm::vec3 pos, float i, glm::vec3 rv, glm::vec3 rvdot)
		{
			float rd = 0.7f;
			float n = Ca;
			float m = Cr;
			float C = n/(n-m) * (n/m) * (m/(n-m));
			

			float er = glm::dot(rv, rvdot);
			glm::vec3 v = rvdot - rv * er / glm::length(rv);
			glm::vec3 vtd = alpha * v / glm::length(v);
			float friction = (fr * er * pos[i] / glm::length(rv));

			float ans_1 = -powf(rd, n) * n * pos[i] * powf((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z), (-n / 2 - 1));
			float ans_2 = powf(rd, m) * m * pos[i] * powf((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z), (-m / 2 - 1));

			return -(ans_1+ans_2) - friction - fr * (v[i] - vtd[i]);
		}
		static void Center_Revolution(mesh* obj)
		{
			obj->transform.rotation = obj->transform.rotation + glm::vec3(0, 0.1, 0);
		}
		static void Sum_Contained(std::vector<mesh*>& All_Mesh,Log &myLogger)
		{
			float Sum = 0;
			for (int i = 0; i < All_Mesh.size(); i++)
			{
				for (int j = 0; j < All_Mesh.size(); j++)
				{
					if (i != j)
					{
						float sum = glm::length(All_Mesh[i]->transform.position - All_Mesh[j]->transform.position);
						Sum += sum;
					}
				}
			}

			std::string time = std::to_string(wt);
			std::string pos = std::to_string(Sum);
			myLogger.logMessage(time, pos);

			All_vector_time.push_back(wt);
			All_Vector_Sum.push_back(Sum);
			cur_space_bound_check = Sum;
			max_space_bound_check = (max_space_bound_check < cur_space_bound_check) ? cur_space_bound_check : max_space_bound_check;
			min_space_bound_check = (min_space_bound_check > cur_space_bound_check) ? cur_space_bound_check : min_space_bound_check;
		}
		static void Boundary(Transforms &transform)
		{
			if (glm::length(boundary_limits) > 0)
			{
				if (abs(transform.position.x) > boundary_limits.x)
				{
					transform.position.x = boundary_limits.x * (transform.position.x / abs(transform.position.x));
					transform.velocity.x *= -1 * damping;
				}
				if (abs(transform.position.y) > boundary_limits.y)
				{
					transform.position.y = boundary_limits.y * (transform.position.y / abs(transform.position.y));
					transform.velocity.y *= -1 * damping;
				}
				if (abs(transform.position.z) > boundary_limits.z)
				{
					transform.position.z = boundary_limits.z * (transform.position.z / abs(transform.position.z));
					transform.velocity.z *= -1 * damping;
				}
			}
		}
		static glm::vec3 Smoothing_Function(float dis,glm::vec3 dir)
		{
			float raduis = 4.0f;
			return glm::vec3(dir*std::max(0.0f, raduis * raduis - dis * dis));
		}
		static void Avoidance(std::vector<mesh*>& All_Mesh)
		{
			for (int i = 0; i < All_Mesh.size(); i++)
			{
				for (int j = 0; j < All_Mesh.size(); j++)
				{
					if (i != j)
					{
						float dis =6*glm::distance(All_Mesh[i]->transform.position, All_Mesh[j]->transform.position);
						glm::vec3 dir = glm::normalize(All_Mesh[i]->transform.position - All_Mesh[j]->transform.position);
						All_Mesh[i]->transform.force = Smoothing_Function(dis, dir);
					}

				}
			}

		}

		static float calculateFuelConsumption(std::vector<mesh*>& All_Mesh, double totalTime, double timeStep) {
			double totalDistance = 0.0;
			int numSteps = static_cast<int>(totalTime / timeStep);

			for (auto obj : All_Mesh) {
				double distance = 0.0;
				glm::vec3 previousVelocity = obj->transform.velocity; // Initial velocity

				for (int k = 0; k < numSteps; ++k) {
					glm::vec3 currentVelocity = obj->transform.velocity; // Update this if velocity changes over time
					double timeDelta = timeStep;

					// Calculate the distance using the trapezoidal rule
					double sk = (timeDelta / 2.0) * (glm::length(previousVelocity) + glm::length(currentVelocity));
					distance += sk;

					// Update previousVelocity to currentVelocity for the next iteration
					previousVelocity = currentVelocity;
				}

				totalDistance += distance;
			}
			return totalDistance;
		}
		static glm::vec2 stereographic_projection(glm::vec3 point)
		{
			return glm::vec2((real_min_distance * point.x) / (point.z + real_min_distance), (real_min_distance * point.y) / (point.z + real_min_distance));
		}
};
#endif
