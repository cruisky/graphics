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
	float floatSliderValue1 = 10.f;
	float floatSliderValue2 = 50.f;
	int intSliderValue = 42;
	int radioValue = 0;
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
		GUI::Button("Button 0");
		GUI::Button("Button 1");
		GUI::Button("Button 2");
		GUI::FloatSlider("FloatSlider1", floatSliderValue1, 0, 100);

		GUI::EndWindow();

		GUI::BeginWindow("Window 1", window[1]);

		GUI::FloatSlider("FloatSlider1 (with fixed step)", floatSliderValue1, -100, 100, 10.f);
		GUI::FloatSlider("FloatSlider2", floatSliderValue2, 0, 10);
		GUI::IntSlider("IntSlider (with fixed step)", intSliderValue, 0, 100, 17);
		GUI::IntSlider("IntSlider", intSliderValue, 0, 100);
		
		GUI::RadioButton("Item 0", radioValue, 0);
		GUI::RadioButton("Item 1", radioValue, 1);
		GUI::RadioButton("Item 2", radioValue, 2);
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