#pragma once

#include <Common/ITexture.hpp>
#include "GL/glew.h"

namespace Graphics
{
        class Texture : public Common::ITexture
        {
                public:
                        Texture();
                        virtual ~Texture();

                        int Load(std::string path, int nbWidth = -1, int nbHeight = -1) override;
                        void Use(int x = -1, int y = -1) override;

                private:
                        GLuint texture;
                        static GLuint currentTexture;
        };
}