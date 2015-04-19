#pragma once

#include "Application.h"
#include "fwddecl.h"

namespace Cruisky{
	namespace RayTracer{
		class GUIViewer : public Application {
		public:
			GUIViewer(Film *f) : film_(f), rendered(false){}

			void Config();
			bool Render();

		private:
			Film *film_;
			bool rendered;
		};
	}
}