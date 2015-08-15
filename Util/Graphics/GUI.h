#pragma once

#include "System/Input.h"
#include "Math/Geometry.h"
#include "Color.h"
#include "FontMap.h"

namespace TX {
	namespace UI {
		namespace GUI {
			struct Style{
				enum Palette {
					Background,
					Foreground,
					Text,
					Hint,
					Accent,
					AccentHighlight,
					AccentActive,
					COUNT
				};
				float		WindowPadding;
				float		WidgetPadding;
				float		ButtonPadding;		// horizontal padding of a button
				float		TextPadding;		// vertical padding of text to the bottom
				float		LineHeight;
				Color		Colors[Palette::COUNT];
				FontMap		*Font;
				Style(){
					WindowPadding = 18;
					WidgetPadding = 6;
					ButtonPadding = 10;
					LineHeight = 20;

					Colors[Palette::Background] = Color::RGBA(0x00000099);
					Colors[Palette::Foreground] = Color::RGB(0xFFFFFF);
					Colors[Palette::Text] = Color::RGB(0xFFFFFF);
					Colors[Palette::Hint] = Color::RGB(0x616161);
					Colors[Palette::Accent] = Color::RGB(0x00B0FF);				// Light Blue A400
					Colors[Palette::AccentHighlight] = Color::RGB(0x40C4FF);	// Light Blue A200
					Colors[Palette::AccentActive] = Color::RGB(0x80D8FF);		// Light Blue A100
				}
			};

			void Init(FontMap& font);
			void Shutdown();
			void BeginFrame(const Input& input);
			void EndFrame();
			void BeginWindow(const char *name, Rect& window);
			void EndWindow();

			bool Button(const char *name, bool enabled = true);
			bool FloatSlider(const char *name, float *pos, float min, float max, float step = 0.f);
		}
	}
}