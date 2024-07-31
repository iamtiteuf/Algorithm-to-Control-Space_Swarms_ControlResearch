#ifndef COLOR_CLASS_H
#define COLOR_CLASS_H
#include "Glew_Initialization.h"
class Color_Picking
{
public:
	void Init(GLuint width, GLuint height, glm::vec3 position)
	{
		this->width = width;
		this->height = height;

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Color attachment
		glGenTextures(1, &m_pickingTexture);
		glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);

		// Depth buffer
		glGenRenderbuffers(1, &m_depthTexture);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);

		// Check framebuffer is complete
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not complete: " << status << "\n";
		}

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	~Color_Picking()
	{
		glDeleteTextures(1, &m_pickingTexture);
		glDeleteRenderbuffers(1, &m_depthTexture);
		glDeleteFramebuffers(1, &m_fbo);
	}
	void EnableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	}
	void DisableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}


	glm::vec4 Read_Pixels()
	{
		double X;
		double Y;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(Main_Window , &X, &Y);
		int display_w, display_h;
		glfwGetFramebufferSize(Main_Window , &display_w, &display_h);

		

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		
		unsigned char pixel[4];


		glReadPixels((int)X, display_h - (int)Y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

		/*std::cout << "R: " << Round_Read_Pixel((float)pixel[0] / 255.0f) << "\t";
		std::cout << "G: " << Round_Read_Pixel((float)pixel[1] / 255.0f) << "\t";
		std::cout << "B: " << Round_Read_Pixel((float)pixel[2] / 255.0f) << "\t";
		std::cout << "A: " << Round_Read_Pixel((float)pixel[3] / 255.0f) << "\n";*/

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return glm::vec4
		(
			Round_Read_Pixel((float)pixel[0] / 255.0f),
			Round_Read_Pixel((float)pixel[1] / 255.0f),
			Round_Read_Pixel((float)pixel[2] / 255.0f),
			Round_Read_Pixel((float)pixel[3] / 255.0f)
		);
	}
private:

	float Round_Read_Pixel(float value)
	{
		return roundf(value * 100) / 100;
	}

	GLuint width, height, m_pickingTexture, m_depthTexture, m_fbo;



};
#endif