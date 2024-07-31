#ifndef SHADER_CLASS
#define SHADER_CLASS
#include "Glew_Initialization.h"

class Shaders
{
public:
    GLuint shaderprogram;
    void Create_Normal_Shaders()
        {
            
            const GLchar* fragmentSourceCode =
                R"(
                    #version 330 core
                     out vec4 FragColor;  
                     in vec3 Color;
  
                     void main()
                       {
                          FragColor = vec4(Color,1.0f);
                       }
                  )";
            const GLchar* vertexSourceCode =
                R"(  #version 330 core
                     layout (location = 0) in vec3 aPos;
                     layout (location = 1) in vec3 color;
                     out vec3 Color;
                     out vec2 TexCord;

                     uniform mat4 CamMatrix;
                     uniform mat4 model;
                     uniform int outline;
                    
                     void main()
                       {
                         gl_Position = CamMatrix*model*vec4(aPos, 1.0f);
                         Color = color;
                         if(outline == 1)
                         {Color = vec3(0.1,0.1,0.1);} else {  Color = color;}
                       })";

            GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexshader, 1, &vertexSourceCode, nullptr);
            glCompileShader(vertexshader);

            GLint success;
            glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
            if (!success) {
                GLchar infoLog[512];
                glGetShaderInfoLog(vertexshader, 512, nullptr, infoLog);
                std::cerr << "Failed to compile vertex shader: " << infoLog << "\n";
                return;
            }

            GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentshader, 1, &fragmentSourceCode, nullptr);
            glCompileShader(fragmentshader);

            glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
            if (!success) {
                GLchar infoLog[512];
                glGetShaderInfoLog(fragmentshader, 512, nullptr, infoLog);
                std::cerr << "Failed to compile fragment shader: " << infoLog << "\n";
                return;
            }

            shaderprogram = glCreateProgram();
            glAttachShader(shaderprogram, vertexshader);
            glAttachShader(shaderprogram, fragmentshader);
            glLinkProgram(shaderprogram);

            glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
            if (!success) {
                GLchar infoLog[512];
                glGetProgramInfoLog(shaderprogram, 512, nullptr, infoLog);
                std::cerr << "Failed to link shader program: " << infoLog << "\n";
                return;
            }

            if (shaderprogram != 0) {
                std::cout << "Program created\n";
            }
            else {
                std::cout << "Program not created\n";
            }

            glDeleteShader(vertexshader);
            glDeleteShader(fragmentshader);
        }
    void Create_Picking_Shaders()
    {

        const GLchar* fragmentSourceCode =
            R"(
#version 330 core
layout(location = 0) out vec4  FragColor;
in vec4 F_ColorID; 


void main() 
{
   FragColor =  vec4(F_ColorID);
}

 )";
        const GLchar* vertexSourceCode =
            R"( 
#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 F_ColorID;

uniform mat4 CamMatrix;
uniform mat4 model;

uniform vec4 aColor;


void main()
{
    gl_Position = CamMatrix*model*vec4(aPos, 1.0f);
    F_ColorID = aColor;
})";

        GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexshader, 1, &vertexSourceCode, nullptr);
        glCompileShader(vertexshader);

        GLint success;
        glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(vertexshader, 512, nullptr, infoLog);
            std::cerr << "Failed to compile vertex shader: " << infoLog << "\n";
            return;
        }

        GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentshader, 1, &fragmentSourceCode, nullptr);
        glCompileShader(fragmentshader);

        glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(fragmentshader, 512, nullptr, infoLog);
            std::cerr << "Failed to compile fragment shader: " << infoLog << "\n";
            return;
        }

        shaderprogram = glCreateProgram();
        glAttachShader(shaderprogram, vertexshader);
        glAttachShader(shaderprogram, fragmentshader);
        glLinkProgram(shaderprogram);

        glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(shaderprogram, 512, nullptr, infoLog);
            std::cerr << "Failed to link shader program: " << infoLog << "\n";
            return;
        }

        if (shaderprogram != 0) {
            std::cout << "Program created\n";
        }
        else {
            std::cout << "Program not created\n";
        }

        glDeleteShader(vertexshader);
        glDeleteShader(fragmentshader);
    }

        ~Shaders()
        {
            glDeleteProgram(shaderprogram);
        }
        void useprogram() { glUseProgram(shaderprogram); }
        void disableprogram() { glUseProgram(0); }
        void setMat4fv(const char* uniform, glm::mat4 matrix)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderprogram, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }// check if not working

        void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(shaderprogram, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, const glm::vec4& value) const
        {
            glUniform4fv(glGetUniformLocation(shaderprogram, name.c_str()), 1, &value[0]);
        }
};
#endif

