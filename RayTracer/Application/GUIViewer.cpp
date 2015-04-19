#include "stdafx.h"
#include "GUIViewer.h"
#include "Core/Film.h"

namespace Cruisky{
	namespace RayTracer{
		
		void GUIViewer::Config(){
			strcpy_s(config.title, "RayTracer");
			config.width = film_->Width();
			config.height = film_->Height();
			config.fixsize = true;
		}

		bool GUIViewer::Render(){
			if (!rendered){
				glDrawPixels(config.width, config.height, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
				rendered = true;
				return true;
			}
			return false;
		}
	}
}