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
				Color		Colors[Palette::COUNT];
				FontMap		*Font;
				Style(){
					WindowPadding = 18;
					WidgetPadding = 10;

					Colors[Palette::Background] = Color::RGBA(0x0000007F);
					Colors[Palette::Foreground] = Color::RGB(0xFFFFFF);
					Colors[Palette::Text] = Color::RGB(0xFFFFFF);
					Colors[Palette::Hint] = Color::RGB(0x616161CC);
					Colors[Palette::Accent] = Color::RGBA(0x01579BCC);				// Light Blue 900
					Colors[Palette::AccentHighlight] = Color::RGBA(0x0277BDCC);		// Light Blue 800
					Colors[Palette::AccentActive] = Color::RGBA(0x039BE5CC);		// Light Blue 600
				}
			};

			Style& GetStyle();
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