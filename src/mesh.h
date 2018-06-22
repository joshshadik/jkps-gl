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

            void render(Material* replacementMaterial = nullptr);
            void render(const glm::mat4& mtx, Material* replacementMaterial = nullptr);

            void setInstances(uint32_t instances);

        private:
            std::vector<Geometry*> _geometries;
            Material* _renderMaterial;

            GLint _modelUniformLocation;
            uint32_t _instanceCount = 1;
        };

    }
}