#include "..\include\swapchain.h"

using namespace jkps::gl;

jkps::gl::Swapchain::Swapchain(std::array<Framebuffer*, 2> fbos)
	: _fbos(fbos)
{
}

Framebuffer * jkps::gl::Swapchain::front()
{
	return _fbos[_front];
}

Framebuffer * jkps::gl::Swapchain::back()
{
	return _fbos[_back];
}

void jkps::gl::Swapchain::swap()
{
	int f = _front;
	_front = _back;
	_back = f;
}
