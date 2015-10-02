#include "Rendering/Obj.h"
#include "System/Application.h"

namespace TX
{
	class ObjViewer : public UI::Application {
	public:
		void Start(){

		}
		bool Render(){
			return true;
		}
	};
}

int main(){
	TX::ObjViewer().Run();
	return 0;
}