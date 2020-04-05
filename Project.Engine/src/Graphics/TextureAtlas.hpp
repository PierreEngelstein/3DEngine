#pragma once

#include <vector>
#include <Common/ITexture.hpp>
#include "GL/glew.h"

namespace Graphics
{
        class TextureAtlas : public Common::ITexture
        {
                public:
                        TextureAtlas();
                        virtual ~TextureAtlas();

                        int Load(std::string path, int nbWidth, int nbHeight) override;
                        void Use(int x, int y) override;

                private:
                        void Use(int index);
                        /* Information about sprite  */
                        int tileWidth;
                        int tileHeight;
                        int nbTileWidth;
                        int nbTileHeight;
                        /* The list of all opengl textures (more efficient than storing the whole image) */
                        std::vector<GLuint> textures;

        };
}