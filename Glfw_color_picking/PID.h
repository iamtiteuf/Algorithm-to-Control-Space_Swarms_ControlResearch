#ifndef PID_H
#define PID_H
#include "Glew_Initialization.h"
class PID
{
private:
    glm::vec3  kp = glm::vec3 (0,0,0); //proportional gain
    glm::vec3  ki = glm::vec3 (0,0,0); //intergral gain
    glm::vec3  kd = glm::vec3 (0,0,0); //Derivative gain

    glm::vec3 prevError = glm::vec3(0, 0, 0);
    glm::vec3 integral = glm::vec3(0, 0, 0);

    glm::vec3  calculate(glm::vec3  setpoint, glm::vec3  feedback) {
        glm::vec3  error = setpoint - feedback;

        // Proportional term
        glm::vec3  proportional = kp * error;

        // Integral term
        integral += error;
        glm::vec3  integralTerm = ki * integral;

        // Derivative term
        glm::vec3  derivative = kd * (error - prevError);
        prevError = error;

        // Calculate the control outputpidpid
        glm::vec3  output = proportional + integralTerm + derivative;

        return output;
    }
public:
    /*PID(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        kp = a;
        ki = b;
        kd = c; 
        glm::vec3 prevError = glm::vec3(0, 0, 0);
        glm::vec3 integral = glm::vec3(0, 0, 0);
    }*/
    void PID_Controller(glm::vec3 targetPosition, glm::vec3 & objectPosition) 
    {
        for (int i = 0; i < 100; ++i) {
            // Get control output
            glm::vec3 controlOutput = calculate(targetPosition, objectPosition);

            // Apply the control output to update the object's position
            objectPosition += controlOutput;

            std::cout << "Iteration " << i + 1 << ": Target Position=(" << targetPosition.x << ", " << targetPosition.y << ", " << targetPosition.z
                << "), Object Position=(" << objectPosition.x << ", " << objectPosition.y << ", " << objectPosition.z
                << "), Control Output=(" << controlOutput.x << ", " << controlOutput.y << ", " << controlOutput.z << ")" << std::endl;
        }
    }
};
#endif
