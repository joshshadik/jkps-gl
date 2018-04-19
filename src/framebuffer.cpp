#include "framebuffer.h"

using namespace jkps::gl;

jkps::gl::Framebuffer::Framebuffer()
{
    glCreateFramebuffers(1, &_fboId);
}

void jkps::gl::Framebuffer::setup(const std::vector<std::shared_ptr<Texture>>& color, std::shared_ptr<Texture> depth, const glm::ivec2 & size)
{
    _color = color;
    _depth = depth;
    _size = size;

    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
    for (int i = 0; i < _color.size(); ++i )
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _color[i]->id(), 0);
    }

    if (_depth)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->id(), 0);
    }
}

void jkps::gl::Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
    glViewport(0, 0, _size.x, _size.y);

    std::vector<GLenum> attachments;
    for (int i = 0; i < _color.size(); ++i )
    {
        attachments.push_back((GLenum)(GL_COLOR_ATTACHMENT0 + i));
    }
    glDrawBuffers(attachments.size(), attachments.data());
}

void jkps::gl::Framebuffer::bindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void jkps::gl::Framebuffer::bindDefault(const glm::ivec2 & size)
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glViewport(0, 0, size.x, size.y);
}
