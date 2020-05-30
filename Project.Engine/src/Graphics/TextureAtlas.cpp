#include "TextureAtlas.hpp"
#include <Common/lodepng/lodepng.h>
#include <Logging/Logger.hpp>

namespace Graphics {
    TextureAtlas::TextureAtlas() {
        this->tileWidth = 0;
        this->tileHeight = 0;
        this->nbTileWidth = 0;
        this->nbTileHeight = 0;
    }

    TextureAtlas::~TextureAtlas() {
        LOG_INFO("Cleared texture atlas\n")
        for (GLuint tex : textures)
            glDeleteTextures(1, &tex);
        textures.clear();
    }

    int TextureAtlas::Load(std::string path, int nbWidth, int nbHeight) {
        this->nbTileWidth = nbWidth;
        this->nbTileHeight = nbHeight;
        std::vector<unsigned char> data;
        unsigned width, height;
        unsigned error = lodepng::decode(data, width, height, path.c_str());
        if (error) {
            LOG_FATAL("Failed to open png file %s : %s\n", path.c_str(), lodepng_error_text(error))
        }

        if ((width % this->nbTileWidth)) {
            LOG_ERROR("Error when creating texture atlas %s. nbWidth (%d) is incorrect, must be a divisor of width (%d)", path.c_str(), nbWidth, width);
            return 2;
        }

        if ((height % this->nbTileHeight)) {
            LOG_ERROR("Error when creating texture atlas %s. nbHeight (%d) is incorrect, must be a divisor of height (%d)\n", path.c_str(), nbHeight, height);
            return 2;
        }

        this->tileWidth = width / this->nbTileWidth;
        this->tileHeight = height / this->nbTileHeight;

        LOG_DEBUG("Atlas contains %d tiles using size [%d,d]\n", nbTileWidth * nbTileHeight, tileWidth, tileHeight);

        for (int tileY = 0; tileY < this->nbTileHeight; tileY++) {
            for (int tileX = 0; tileX < this->nbTileWidth; tileX++) {
                std::vector<unsigned char> img;
                for (int y = 0; y < tileHeight; y++) {
                    for (int x = 0; x < tileWidth; x++) {
                        int index_global = ((x + tileX * this->tileWidth) + (y + tileY * this->tileHeight) * width) * 4;
                        int r = data[index_global];
                        int g = data[index_global + 1];
                        int b = data[index_global + 2];
                        int a = data[index_global + 3];
                        img.push_back(r);
                        img.push_back(g);
                        img.push_back(b);
                        img.push_back(a);
                    }
                }
                /* Generate the OpenGL texture and push it back to the list of textures */
                GLuint texture = 0;
                glGenTextures(1, &texture);
                glEnable(GL_TEXTURE_2D);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tileWidth, this->tileHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             &img[0]);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                this->textures.push_back(texture);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }

        return 0;
    }

    void TextureAtlas::Use(int index) {
        if (index < this->textures.size() && index >= 0) {
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->textures[index]);
        }
    }

    void TextureAtlas::Use(int x, int y) {
        this->Use(x + y * this->nbTileWidth);
    }

}