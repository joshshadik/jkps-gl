#pragma once

#include "framebuffer.h"

#include <array>

namespace jkps
{
	namespace gl
	{
		class Swapchain
		{
		public:
			Swapchain(std::array<Framebuffer*, 2> fbos);

			Framebuffer * front();
			Framebuffer* back();

			void swap();

		private:
			std::array<Framebuffer*, 2> _fbos;

			int _front = 0;
			int _back = 1;
		};
	}
}

