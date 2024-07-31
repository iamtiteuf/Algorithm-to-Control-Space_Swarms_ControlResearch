#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Glew_Initialization.h"

class Camera
{
public:
	glm::mat4 Camera_Matrix()
	{


		glfwGetWindowSize(Main_Window , &width, &height);
		float FOVdeg = 45.0f;
		float nearPlane = 0.1f;
		float farplane = 200000.0f;
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::lookAt(Camera_Position, Camera_Position + Orientation, Up);
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farplane);
		//glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

		

		return projection * view;
	}
	void Camera_Controls(bool IsActive)
	{
		if (IsActive)
		{
			glfwGetFramebufferSize(Main_Window , &width, &height);

			if (glfwGetKey(Main_Window , GLFW_KEY_W) == GLFW_PRESS)
			{
				Camera_Position += speed * Orientation;
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_A) == GLFW_PRESS)
			{
				Camera_Position += speed * -glm::normalize(glm::cross(Orientation, Up));
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_S) == GLFW_PRESS)
			{
				Camera_Position += speed * -Orientation;
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_D) == GLFW_PRESS)
			{
				Camera_Position += speed * glm::normalize(glm::cross(Orientation, Up));
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				Camera_Position += speed * Up;
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				Camera_Position += speed * -Up;
			}
			if (glfwGetKey(Main_Window , GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				speed = 0.4f;
			}
			else if (glfwGetKey(Main_Window , GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			{
				speed = 0.1f;
			}
			// Handles mouse inputs
			if (glfwGetMouseButton(Main_Window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS )
			{
				// Hides mouse cursor
				glfwSetInputMode(Main_Window , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				// Prevents camera from jumping on the first click
				if (firstClick)
				{
					glfwGetCursorPos(Main_Window , &mouseX2, &mouseY2);
					glfwSetCursorPos(Main_Window , (width / 2), (height / 2));
					firstClick = false;
				}

				// Stores the coordinates of the cursor
				double mouseX;
				double mouseY;
				// Fetches the coordinates of the cursor
				glfwGetCursorPos(Main_Window , &mouseX, &mouseY);
				// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
				// and then "transforms" them into degrees 
				float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
				float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

				// Calculates upcoming vertical change in the Orientation
				glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

				// Decides whether or not the next vertical Orientation is legal or not
				if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
				{
					Orientation = newOrientation;
				}

				// Rotates the Orientation left and right
				Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

				// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
				glfwSetCursorPos(Main_Window , (width / 2), (height / 2));
			}
			else if (glfwGetMouseButton(Main_Window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE )
			{

				// Unhides cursor since camera is not looking around anymore
				if (!firstClick)
				{
					glfwSetInputMode(Main_Window , GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					glfwSetCursorPos(Main_Window , mouseX2, mouseY2);
				}
				// Makes sure the next time the camera looks around it doesn't jump
				firstClick = true;
			}
		}
	}
private:
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	double mouseX2;
	double mouseY2;
	int width, height;

	bool firstClick = false;

	bool done = false;
	float speed = 0.1f;
	float sensitivity = 100.0f;

};
#endif