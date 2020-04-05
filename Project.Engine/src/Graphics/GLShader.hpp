#pragma once

#include <Common/IShader.hpp>
#include <unordered_map>

namespace Graphics
{
    class GLShader : public Common::IShader
    {
        public:
            GLShader();
            ~GLShader();
            virtual bool AddProgram(const char *path, Common::ShaderType type) override;
            virtual bool GenerateShader() override;
            virtual bool GenerateUniforms(std::vector<std::string> uniforms) override;
            virtual void DeleteShader() override;

            virtual inline void Use() override;
            virtual inline void Stop() override;

            virtual unsigned int GetProgram() override;

            virtual inline int AssignUniform(std::string uniform, glm::mat4x4 matrix) override;
            virtual inline int AssignUniform(std::string uniform, glm::vec4 vector) override;
            virtual inline int AssignUniform(std::string uniform, glm::vec3 vector) override;
            virtual inline int AssignUniform(std::string uniform, glm::vec2 vector) override;
            virtual inline int AssignUniform(std::string uniform, int integer) override;
            virtual inline int AssignUniform(std::string uniform, float val) override;
        
        private:
            unsigned int m_program;
            std::unordered_map<std::string, int> m_uniforms;
            std::vector<unsigned int> m_compiled_programs;
            static unsigned int previous_program;
    };
}