#pragma once

#include "txbase/opengl/input.h"
#include "txbase/math/geometry.h"
#include "txbase/math/color.h"
#include "txbase/gui/fontmap.h"

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
				float		LineHeight;
				float		StrokeWidth;
				float		ScrollSpeed;
				Color		Colors[Palette::COUNT];
				FontMap*	Font;

				// -------- auto adjusted -------
				float		WindowPadding;
				float		WidgetPadding;
				float		TextPaddingX;		// horizontal padding of text (similar to tab)
				float		TextPaddingY;		// vertical padding of text to the bottom
				float		FormWidgetRadius;		// radio button, checkbox
				float		FormWidgetSelectedRadius;

				Style(){
					WindowPadding = 18;
					LineHeight = 20;
					StrokeWidth = 3.f;
					ScrollSpeed = 60;

					Colors[Palette::Background] = Color::RGBA(0x212121BD);
					Colors[Palette::Foreground] = Color::RGB(0xFFFFFF);
					Colors[Palette::Text] = Color::RGB(0xFFFFFF);
					Colors[Palette::Hint] = Color::RGBA(0xBDBDBD7F);
					Colors[Palette::Accent] = Color::RGBA(0x0091EABD);			// Light Blue A700
					Colors[Palette::AccentHighlight] = Color::RGBA(0x00B0FFBD);	// Light Blue A400
					Colors[Palette::AccentActive] = Color::RGBA(0x80D8FFBD);	// Light Blue A100
				}
				void Update(){
					if (Font){
						WindowPadding = Math::Max(WindowPadding, Font->Height() + 4.f);	// adjust window padding according to font height
						WidgetPadding = LineHeight * 0.4f;
						TextPaddingX = Font->Height() * 0.5f;
						TextPaddingY = (WindowPadding - Font->Height()) * 0.9f;
					}
					FormWidgetRadius = LineHeight * 0.4f;
					FormWidgetSelectedRadius = FormWidgetRadius * 0.5f;
				}
				inline float HalfLineHeight() const { return LineHeight * 0.5f; }
			};

			Style& GetStyle();
			void Init(FontMap& font);
			void Shutdown();
			void BeginFrame(Input& input);
			void EndFrame();
			bool BeginWindow(const std::string& name, Rect& window);
			void EndWindow();

			void Divider();
			void Text(const std::string& text, bool isHint = false);
			bool Button(const std::string& name, bool enabled = true);
			bool FloatSlider(const std::string& name, float& val, float min, float max, float step = 0.f);
			bool IntSlider(const std::string& name, int& val, int min, int max, int step = 0);
			bool ColorSlider(const std::string& name, Color &col, Color::Channel channel = Color::Channel::RGB);
			bool RadioButton(const std::string& name, int& val, int itemVal);
			bool CheckBox(const std::string& name, bool& val);
			void ProgressBar(const std::string& name, const float& zero2One);
			bool TextField(const std::string& name, std::string& text, bool selectAllOnActive = false);
		}
	}
}
