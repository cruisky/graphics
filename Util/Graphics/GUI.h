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
				float		LineHeight;
				Color		Colors[Palette::COUNT];

				// -------- auto adjusted -------
				float		TextPaddingX;		// horizontal padding of text (similar to tab)
				float		TextPaddingY;		// vertical padding of text to the bottom
				float		FormWidgetRadius;		// radio button, checkbox
				float		FormWidgetSelectedRadius;

				FontMap		*Font;
				Style(){
					WindowPadding = 18;
					WidgetPadding = 8;
					LineHeight = 20;

					Colors[Palette::Background] = Color::RGBA(0x212121BD);
					Colors[Palette::Foreground] = Color::RGB(0xFFFFFF);
					Colors[Palette::Text] = Color::RGB(0xFFFFFF);
					Colors[Palette::Hint] = Color::RGBA(0xFFFFFF7F);
					Colors[Palette::Accent] = Color::RGBA(0x0091EABD);			// Light Blue A700
					Colors[Palette::AccentHighlight] = Color::RGBA(0x00B0FFBD);	// Light Blue A400
					Colors[Palette::AccentActive] = Color::RGBA(0x80D8FFBD);	// Light Blue A100
				}
				void Update(){
					if (Font){
						WindowPadding = Math::Max(WindowPadding, Font->Height() + 4.f);	// adjust window padding according to font height
						TextPaddingX = Font->Height() * 0.5f;
						TextPaddingY = (WindowPadding - Font->Height()) * 0.9f;
					}
					FormWidgetRadius = LineHeight * 0.4f;
					FormWidgetSelectedRadius = FormWidgetRadius * 0.5f;
				}
			};

			void Init(FontMap& font);
			void Shutdown();
			void BeginFrame(const Input& input);
			void EndFrame();
			void BeginWindow(const char *name, Rect& window);
			void EndWindow();

			void Divider();
			bool Button(const char *name, bool enabled = true);
			bool FloatSlider(const char *name, float& val, float min, float max, float step = 0.f);
			bool IntSlider(const char *name, int& val, int min, int max, int step = 0);
			bool RadioButton(const char *name, int& val, int itemVal);
			bool CheckBox(const char *name, bool& val);
		}
	}
}