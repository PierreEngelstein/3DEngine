#include <Graphics/GLShader.hpp>
#include "GL/glew.h"
#include <Common/Utils.h>
#include <Logging/Logger.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace Graphics
{
    unsigned int GLShader::previous_program = 0;
    GLShader::GLShader()
    {

    }
    
    GLShader::~GLShader()
    {
        LOG_INFO("Cleared shader.\n");
        DeleteShader();
        m_uniforms.clear();
    }

    bool GLShader::AddProgram(const char *path, Common::ShaderType type)
    {
        GLenum shader_type;
        switch (type)
        {
            case Common::VERTEX :
            {
                shader_type = GL_VERTEX_SHADER;
                break;
            }
            case Common::FRAGMENT:
                shader_type = GL_FRAGMENT_SHADER;
                break;
            default:
                LOG_ERROR("Wrong shader type !\n")
                return false;
        }

        GLuint shader = glCreateShader(shader_type);
        std::string shader_text;
        if(!LoadFile(std::string(path), &shader_text))
        {
            LOG_ERROR("Failed to load shader from %s\n", path)
            return false;
        }
        const char *shader_str = shader_text.c_str();
        glShaderSource(shader, 1, &(shader_str), NULL);
        glCompileShader(shader);
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
            LOG_ERROR("Failed to compile shader : %s\n", errorLog.data())
            return false;
        }

        m_compiled_programs.push_back(shader);
        return true;
    }

    bool GLShader::GenerateShader()
    {
        if(m_compiled_programs.size() == 0)
        {
            LOG_ERROR("Failed to generate shader program, no shader found.\n")
            return false;
        }
        m_program = glCreateProgram();
        for(auto i : m_compiled_programs)
            glAttachShader(m_program, i);
        
        glLinkProgram(m_program);

        GLint isCompiled = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetProgramInfoLog(m_program, maxLength, &maxLength, &errorLog[0]);
            LOG_ERROR("Failed to compile program : %s\n", errorLog.data())
            return false;
        }

        glValidateProgram(m_program);
        isCompiled = 0;
        glGetProgramiv(m_program, GL_VALIDATE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetProgramInfoLog(m_program, maxLength, &maxLength, &errorLog[0]);
            LOG_ERROR("Failed to validate program : %s\n", errorLog.data())
            return false;
        }

        for(auto i : m_compiled_programs)
            glDetachShader(m_program, i);
        
        for(auto i : m_compiled_programs)
            glDeleteShader(i);
        
        m_compiled_programs.clear();

        return true;
    }

    bool GLShader::GenerateUniforms(std::vector<std::string> uniforms)
    {
        for(auto uniform : uniforms)
        {
            m_uniforms[uniform] = glGetUniformLocation(m_program, uniform.c_str());
            if(m_uniforms[uniform] < 0)
            {
                LOG_ERROR("Failed to bind uniform %s\n", uniform.c_str())
                return false;
            }
        }
        return true;
    }

    void GLShader::DeleteShader()
    {
        glDeleteProgram(m_program);
        glUseProgram(0);
    }

    inline void GLShader::Use()
    {
        if(m_program != previous_program)
        {
            glUseProgram(m_program);
            previous_program = m_program;
        }
    }

    inline void GLShader::Stop()
    {
        glUseProgram(0);
    }

    unsigned int GLShader::GetProgram()
    {
        return m_program;
    }

    inline int GLShader::AssignUniform(std::string uniform, glm::mat4x4 matrix)
    {
        glUniformMatrix4fv(m_uniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
        return 0;
    }

    inline int GLShader::AssignUniform(std::string uniform, glm::vec3 vector)
    {
        glUniform3f(m_uniforms[uniform], vector.x, vector.y, vector.z);
        return 0;
    }

    inline int GLShader::AssignUniform(std::string uniform, glm::vec4 vector)
    {
        glUniform4f(m_uniforms[uniform], vector.x, vector.y, vector.z, vector.w);
        return 0;
    }

    inline int GLShader::AssignUniform(std::string uniform, glm::vec2 vector)
    {
        glUniform2f(m_uniforms[uniform], vector.x, vector.y);
        return 0;
    }

    inline int GLShader::AssignUniform(std::string uniform, int integer)
    {
        glUniform1i(m_uniforms[uniform], integer);
        return 0;
    }

    inline int GLShader::AssignUniform(std::string uniform, float value)
    {
        glUniform1f(m_uniforms[uniform], value);
        return 0;
    }
}