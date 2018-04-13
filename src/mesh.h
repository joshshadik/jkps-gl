#pragma once

#include "geometry.h"
#include "material.h"
#include <memory>

namespace jkps
{
	namespace gl
	{
		class Mesh
		{
		public:
			Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

			void render();

		private:
			std::shared_ptr<Geometry> _geometry;
			std::shared_ptr<Material> _material;
		};

	}
}