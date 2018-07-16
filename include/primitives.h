#pragma once

#include "geometry.h"

using namespace jkps::gl;

namespace jkps
{
    namespace engine
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