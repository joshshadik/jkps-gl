#pragma once

#include "geometry.h"
#include "material.h"
#include <memory>
#include <vector>

namespace jkps
{
	namespace gl
	{
		class Mesh
		{
		public:
			Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
			Mesh(std::vector<std::shared_ptr<Geometry>> geometries, std::shared_ptr<Material> material);

			void render();

		private:
			std::vector<std::shared_ptr<Geometry>> _geometries;
			std::shared_ptr<Material> _material;
		};

	}
}