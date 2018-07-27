#pragma once

#include "framebuffer.h"
#include "material.h"


using namespace jkps::gl;

namespace jkps
{
    namespace engine
    {
        class Blitter
        {
            public:
                Blitter();
                void init(Material* material, GLint texLoc);

                static void blit(Framebuffer* fbo, Texture* tex);

            private:
                static Blitter* _instance;

                Material* _material;
                GLint _texLoc;
        };

        void blit(Framebuffer* fbo, Texture* tex)
        {
            Blitter::blit(fbo, tex);
        }
    }
}