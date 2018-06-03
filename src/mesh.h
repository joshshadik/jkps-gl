#pragma once

#include "geometry.h"
#include "material.h"
#include <vector>

namespace jkps
{
    namespace gl
    {
        class Mesh
        {
        public:
            Mesh(Geometry* geometry, Material* material);
            Mesh(std::vector<Geometry*> geometries, Material* material);
            Mesh();

            void render();
            void render(const glm::mat4& mtx);

        private:
            std::vector<Geometry*> _geometries;
            Material* _material;

            GLint _modelUniformLocation;
        };

    }
}