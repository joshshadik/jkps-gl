#pragma once

#include "geometry.h"

namespace jkps
{
    namespace gl
    {
        class Primitives
        {
        public:
            static void init();

            static Geometry* quad() { return _quad; }

        private:
            static Geometry* _quad;

        };
    }
}