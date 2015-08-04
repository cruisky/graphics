#include "System/Application.h"
#include "Graphics/GUI.h"

using namespace TX;

class GUIDemo : public Application {
public:
	GUIDemo(){}
	void Start(){}
	void Config(){}
	bool Render(){ return false; }
	void OnMouseButton(Button button, ButtonState state, int x, int y){}
	void OnKey(unsigned char c, int x, int y){}
	void OnSpecialKey(KeyCode code, int x, int y){}
	void OnResize(){}
};

int main(){
	GUIDemo demo;
	demo.Run();
}