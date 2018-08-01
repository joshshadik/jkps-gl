#include "framebuffer.h"


using namespace jkps::gl;
using namespace jkps::util;

jkps::gl::Framebuffer::Framebuffer(NodeList<Texture*> color, Texture* depth, const glm::ivec2 & size)
    : _color(color)
    , _depth(depth)
    , _size(size)
{
    glGenFramebuffers(1, &_fboId);

    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

    int i = 0;
    auto iter = _color.iter();
    if (iter->begin())
    {   
        do
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, iter->current()->id(), 0);
            ++i;
        } while (iter->next());
    }

    if (_depth)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->id(), 0);
    }

    _valid = true;
}

jkps::gl::Framebuffer::Framebuffer()
    : _valid(false)
    , _color(NodeList<Texture*>(nullptr))
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

void jkps::gl::Framebuffer::bindDefaultVP(const glm::ivec4 & viewport)
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

