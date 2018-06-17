#pragma once

#include "framebuffer.h"
#include "mesh.h"

#include <array>

namespace jkps
{
	namespace engine
	{
		class ParticleSystem
		{
		public:
            ParticleSystem();

            void init(gl::Geometry* geometry, gl::Material* renderMaterial, gl::Material* updateMaterial, int maxCountSqRoot);
			void update(double dt);
			void render();

            void lifetime(float lt) { _lifetime = lt; }
            void origin(const glm::vec3& origin) { _origin = origin; }
            void direction(const glm::vec3& direction) { _direction = direction; }
            void magnitude(float magnitude) { _magnitude = magnitude; }
            void randomness(float randomness) { _randomness = randomness; }

		private:
			int _currentCount;
			int _maxCountSqRoot;

			std::array<gl::Framebuffer*, 2> _fbos;

			int _front = 0;
			int _back = 1;

			int _posTexLoc;
			int _velTexLoc;

            int _deltaTimeLoc;
            int _currentTimeLoc;

            int _lifetimeLoc;
            int _originLoc;

            int _directionLoc;
            int _magnitudeLoc;
            int _randomnessLoc;

            int _renderPosTexLoc;
            int _renderTexSizeLoc;

            float _currentTime;
            float _lifetime;
            glm::vec3 _origin;
            glm::vec3 _direction;
            float _magnitude;
            float _randomness;

			gl::Mesh* _mesh;
			gl::Material* _renderMaterial;
            gl::Material* _updateMaterial;
		};
	}
}