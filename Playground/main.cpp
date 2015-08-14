#include "System/Application.h"
#include "Graphics/GUI.h"
#include "Graphics/FontMap.h"
#include "Math/Geometry.h"

using namespace TX;
using namespace TX::UI;

class GUIDemo : public Application {
private:
	Input input;
	Rect window[3];
	FontMap font;
	float floatSliderValue;
	float floatSliderValue2;
public:
	GUIDemo(){}
	void Start(){ 
		font.Load("../Assets/DroidSans/DroidSans.ttf", 14.f);
		GUI::Init(font);

		input.SetScreen(config.width, config.height); 
		window[0] = Rect(0, 0, 200, 200);
		window[1] = Rect(200, 100, 500, 400);
		window[2] = Rect(400, 300, 600, 600);
	}
	void Config(){}
	bool Render(){
		glClearColor(0.8f, 0.9f, 1.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		GUI::BeginFrame(input);
		
		GUI::BeginWindow("Window 0", window[0]);
		if (GUI::Button("Button 0")){
			std::cout << "0-0 clicked" << std::endl;
		}
		if (GUI::Button("Button 1")){
			std::cout << "0-1 clicked" << std::endl;
		}
		if (GUI::Button("Button 2")){
			std::cout << "0-2 clicked" << std::endl;
		}
		if (GUI::FloatSlider("FloatSlider1", &floatSliderValue, 0, 100)){
			std::cout << "floatSlider1 changed to " << floatSliderValue << std::endl;
		}

		GUI::EndWindow();

		GUI::BeginWindow("Window 1", window[1]);

		if (GUI::FloatSlider("FloatSlider1", &floatSliderValue, -100, 100)){
			std::cout << "floatSlider1 changed to " << floatSliderValue << std::endl;
		}
		if (GUI::FloatSlider("FloatSlider2", &floatSliderValue2, 0, 10)){
			std::cout << "floatSlider2 changed to " << floatSliderValue2 << std::endl;
		}

		GUI::EndWindow();

		GUI::BeginWindow("Window 2", window[2]);

		GUI::EndWindow();
		
		GUI::EndFrame();
		input.Clear();
		return true;
	}
	void OnMouseMove(int x, int y){ input.SetMouse((float)x, (float)y); }
	void OnMouseButton(MouseButton button, MouseButtonState state, int x, int y){ input.SetButton(button, state); }
	void OnKey(unsigned char c, int x, int y){ input.AddKey(c); }
	void OnSpecialKey(KeyCode code, int x, int y){ input.AddSpecialKey(code); }
	void OnResize(){ 
		input.SetScreen(config.width, config.height); 
		glViewport(0, 0, config.width, config.height);
	}
};

int main(){
	GUIDemo demo;
	demo.Run();
}