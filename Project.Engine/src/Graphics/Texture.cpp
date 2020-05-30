#include "Texture.hpp"
#include <Common/lodepng/lodepng.h>
#include <iostream>
#include <Logging/Logger.hpp>

namespace Graphics {
    GLuint Texture::currentTexture = 0;

    Texture::Texture() {
    }

    Texture::~Texture() {
        LOG_INFO("Cleared texture %d\n", texture)
        glDeleteTextures(1, &texture);
    }

    int Texture::Load(std::string path, int nbWidt, int nbHeight) {
        /* Get the data from the image path */
        std::vector<unsigned char> data;
        unsigned width, height;
        unsigned error = lodepng::decode(data, width, height, path.c_str());
        if (error) {
            LOG_FATAL("Failed to open png file %s : %s\n", path.c_str(), lodepng_error_text(error))
        }

        LOG_DEBUG("Texture size = %d,%d\n", width, height)

        /* Generate the opengl texture */
        glGenTextures(1, &this->texture);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        LOG_INFO("Created texture %s\n", path.c_str());

        return 0;
    }

    // int Texture::Load()
    // {
    //         glGenTextures(1, &this->texture);
    //         glEnable(GL_TEXTURE_2D);
    //         glActiveTexture(GL_TEXTURE0);
    //         glBindTexture(GL_TEXTURE_2D, this->texture);
    //         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //         return 0;
    // }

    // int Texture::DynamicRender(int width, int height, const void *pixels)
    // {
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    //         return 0;
    // }

    // int  Texture::LoadEmpty(int width, int height)
    // {
    //         glGenTextures(1, &this->texture);
    //         glEnable(GL_TEXTURE_2D);
    //         glActiveTexture(GL_TEXTURE0);
    //         glBindTexture(GL_TEXTURE_2D, this->texture);
    //         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //         INFO("Texture", "Created empty texture " << width << " " << height)
    //         return 0;
    // }

    void Texture::Use(int x, int y) {
        if (this->texture != currentTexture) {
            currentTexture = this->texture;
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->texture);
        }

    }
}