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
					WidgetPadding = 4;
					ButtonPadding = 12;
					LineHeight = 20;

					Colors[Palette::Background] = Color::RGBA(0x0000007F);
					Colors[Palette::Foreground] = Color::RGB(0xFFFFFF);
					Colors[Palette::Text] = Color::RGB(0xFFFFFF);
					Colors[Palette::Hint] = Color::RGB(0x616161);
					Colors[Palette::Accent] = Color::RGB(0x01579B);				// Light Blue 900
					Colors[Palette::AccentHighlight] = Color::RGB(0x0277BD);	// Light Blue 800
					Colors[Palette::AccentActive] = Color::RGB(0x039BE5);		// Light Blue 600
				}
			};

			void Init(FontMap& font);
			void Shutdown();
			void BeginFrame(const Input& input);
			void EndFrame();
			void BeginWindow(const char *name, Rect& window);
			void EndWindow();

			bool Button(const char *name, bool enabled = true);
			bool Slider(const char *name, float *pos, float min, float max, float step, bool enabled = true);
		}
	}
}