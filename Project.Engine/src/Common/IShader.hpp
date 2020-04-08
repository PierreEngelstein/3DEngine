#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Common
{
    enum ShaderType
    {
        VERTEX = 1, // Vertex shader
        FRAGMENT = 2 // Fragment shader
    };
    
    /**
     * @brief Base shader interface.
     * 
     */
    class IShader
    {
        public:
            virtual ~IShader(){}
            /**
             * @brief Adds and compile a single shader program.
             * 
             * @param path : The path to the shader source.
             * @param type : The type of shader.
             * @return true if no problem occured.
             * @return false otherwise.
             */
            virtual bool AddProgram(const char *path, ShaderType type) = 0;

            /**
             * @brief Generates the full shader program from previously compiled individual shaders.
             * 
             * @return true if no problem occured.
             * @return false otherwise.
             */
            virtual bool GenerateShader() = 0;

            /**
             * @brief Generates the uniforms for the current program.
             * 
             * @param uniforms : The list of uniform to be generated.
             * @return true if no problem occured.
             * @return false otherwise.
             */
            virtual bool GenerateUniforms(std::vector<std::string> uniforms) = 0;

            /** Deletes the shader from memory. */
            virtual void DeleteShader() = 0;

            /** Tells the graphics API to use the current shader. */
            virtual void Use() = 0;

            /** Tells the graphics API to stop using the current shader. */
            virtual void Stop() = 0;

            /** Gets the current underlying program. */
            virtual unsigned int GetProgram() = 0;

            /** Assigns a mat4x4 uniform. */
            virtual int AssignUniform(std::string uniform, glm::mat4x4 matrix) = 0;
            /** Assigns a vec4 uniform. */
            virtual int AssignUniform(std::string uniform, glm::vec4 vector) = 0;
            /** Assigns a vec3 uniform. */
            virtual int AssignUniform(std::string uniform, glm::vec3 vector) = 0;
            /** Assigns a vec2 uniform. */
            virtual int AssignUniform(std::string uniform, glm::vec2 vector) = 0;
            /** Assigns a integer uniform. */
            virtual int AssignUniform(std::string uniform, int integer) = 0;
            /** Assigns a float uniform. */
            virtual int AssignUniform(std::string uniform, float val) = 0;
    };
}