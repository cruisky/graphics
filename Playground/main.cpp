#include "System/Application.h"
#include "Graphics/GUI.h"
#include "Graphics/Canvas.h"

using namespace TX;
using namespace TX::UI;

class GUIDemo : public Application {
private:
	Input input;
	Rect window;
public:
	GUIDemo(){}
	void Start(){ 
		input.SetScreen(config.width, config.height); 
		window = Rect(200, 100, 500, 400);
	}
	void Config(){}
	bool Render(){
		glClearColor(0.8f, 0.9f, 1.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, config.width, config.height);
		GUI::BeginFrame(input);
		
		DrawRect(Vector2(50, 100), Vector2(250, 150), Color::GREEN);
		GUI::BeginWindow("New Window", window);

		GUI::EndWindow();
		GUI::EndFrame();
		input.Clear();
		return true;
	}
	void OnMouseMove(int x, int y){ input.SetMouse(x, y); }
	void OnMouseButton(MouseButton button, MouseButtonState state, int x, int y){ input.SetButton(button, state); }
	void OnKey(unsigned char c, int x, int y){ input.AddKey(c); }
	void OnSpecialKey(KeyCode code, int x, int y){ input.AddSpecialKey(code); }
	void OnResize(){ input.SetScreen(config.width, config.height); }
};

int main(){
	GUIDemo demo;
	demo.Run();
}