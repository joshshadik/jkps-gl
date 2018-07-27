#include "blitter.h"

#include "primitives.h"



jkps::engine::Blitter* jkps::engine::Blitter::_instance = nullptr;

jkps::engine::Blitter::Blitter()
{
    _instance = this;
}

void jkps::engine::Blitter::init(Material* material, GLint texLoc)
{
    _material = material;
    _texLoc = texLoc;
}


void jkps::engine::Blitter::blit(Framebuffer* fbo, Texture* texture)
{
    fbo->bind();
    _instance->_material->setUniform(_instance->_texLoc, texture);
    _instance->_material->bind();

    Primitives::quad()->bind();
    Primitives::quad()->render(1);

    _instance->_material->unbind();

    Framebuffer::bindDefault();

}