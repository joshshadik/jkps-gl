#include "framebuffer.h"


using namespace jkps::gl;

jkps::gl::Framebuffer::Framebuffer(std::vector<Texture*> color, Texture* depth, const glm::ivec2 & size)
{
    glGenFramebuffers(1, &_fboId);
    _color = color;
    _depth = depth;
    _size = size;

    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
    for (int i = 0; i < _color.size(); ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _color.at(i)->id(), 0);
    }

    if (_depth)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->id(), 0);
    }
}

jkps::gl::Framebuffer::Framebuffer()
    : _valid(false)
{

}

jkps::gl::Framebuffer::~Framebuffer()
{
    if (_valid)
    {
        glDeleteFramebuffers(1, &_fboId);
    }
}

jkps::gl::Framebuffer::Framebuffer(Framebuffer && fb)
    : _fboId(fb._fboId)
    , _size(fb._size)
    , _color(std::move(fb._color))
    , _depth(fb._depth)
    , _valid(fb._valid)
{
    fb._valid = false;
}

Framebuffer& jkps::gl::Framebuffer::operator=(Framebuffer && fb)
{
    if (_valid)
    {
        glDeleteFramebuffers(1, &_fboId);
    }

    _fboId = fb._fboId;
    _size = fb._size;
    _color = std::move(fb._color);
    _depth = fb._depth;
    _valid = fb._valid;

    fb._valid = false;

    return *this;
}

void jkps::gl::Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
    glViewport(0, 0, _size.x, _size.y);

    std::vector<GLenum> attachments(_color.size());
    for (int i = 0; i < _color.size(); ++i )
    {
        attachments[i] = (GLenum)(GL_COLOR_ATTACHMENT0 + i);
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
