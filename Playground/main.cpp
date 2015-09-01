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
	bool checkBoxValue;
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
			
			GUI::Divider();
			
			GUI::FloatSlider("Float Slider 1", floatSliderValue1, 0, 100);
		GUI::EndWindow();

		GUI::BeginWindow("Window 1", window[1]);

			GUI::Text("Text1");
			GUI::Text("Text2");

			GUI::FloatSlider("Float Slider 1 (with fixed step)", floatSliderValue1, -100, 100, 10.f);
			GUI::FloatSlider("Float Slider 2", floatSliderValue2, 0, 10);
			GUI::IntSlider("Int Slider (with fixed step)", intSliderValue, 0, 100, 17);
			GUI::IntSlider("Int Slider", intSliderValue, 0, 100);
		
			GUI::Divider();

			GUI::RadioButton("Item 0", radioValue, 0);
			GUI::RadioButton("Item 1", radioValue, 1);
			GUI::RadioButton("Item 2", radioValue, 2);

			GUI::Divider();

			GUI::CheckBox("Check Box", checkBoxValue);
			
			GUI::Divider();
			
			GUI::Button("Button 1");
		GUI::EndWindow();

		GUI::BeginWindow("IMGUI", window[2]);
			GUI::Text(R"xx(IMGUI is a non-retained mode of graphical interface programming. IMGUI is useful when the interface structure is implicit in the program behavior, rather than being an a priori externalized artifact, and is common in games and possibly other soft real-time graphical applications.
				The GUI toolkit is responsible for drawing and reporting i/o, but does not retain a model of widget structures itself. It may or may not track internal state over time, or have a stateful interface (like OpenGL), or be object-oriented, but in all cases the GUI structure and i/o is driven by application-side processes at runtime.
				A typical use might be to attach GUI elements to 3d objects during rendering, interleaving calls to the IMGUI widgets with the 3d rendering calls, so as to produce all graphical output with a single traversal of the data structures. In cases where organization is needed across multiple GUI elements (for example, name labels that should not overlap), the toolkit may build a representation over time and then be asked to draw everything at once.
				)xx", true);
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