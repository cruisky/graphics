#include "Util.h"
#include "GUI.h"
#include "Canvas.h"
#include "Math/Rect.h"

namespace TX { namespace UI { namespace GUI {
	// Ids that uniquely identify a widget 
	struct Widget {
		int windowId = 0;
		int itemId = 0;
		int index = 0;
		Widget() : windowId(0), itemId(0), index(0){}
		Widget(const Widget& ot) : windowId(ot.windowId), itemId(ot.itemId), index(ot.index){}
		Widget& operator = (const Widget& ot){ windowId = ot.windowId; itemId = ot.itemId; index = ot.index; return *this; }
		bool operator == (const Widget& ot){ return windowId == ot.windowId && itemId == ot.itemId && index == ot.index; }
		bool operator != (const Widget& ot){ return windowId != ot.windowId || itemId != ot.itemId || index != ot.index; }
		void Reset(){ windowId = itemId = index = 0; }
		bool HasValue(){ return windowId; }
		void NextDialog() { windowId++; itemId = index = 0; }
		void NextItem() { itemId++; index = 0; }
		void NextIndex() { index++; }
	};
	std::ostream& operator << (std::ostream& os, const Widget& w)
	{
		return os << w.windowId << "-" << w.itemId << "-" << w.index;
	}

	struct State {
		Input input;
		Widget current;
		Widget hot;
		Widget hotToBe;
		Widget active;
		Style style;

		Vector2 drag;	// can be either mouse offset relative to a widget, or the total amount
	};
	static State state;

	////////////////////////////////////////////////////////////////////
	// Helper declaration
	////////////////////////////////////////////////////////////////////

	bool IsHot();
	bool IsActive();
	void SetHot();
	void SetActive();
	void ClearActive();
	bool CheckMouse(MouseButton button, MouseButtonState buttonState);

	////////////////////////////////////////////////////////////////////
	// API implementation
	////////////////////////////////////////////////////////////////////

	void BeginFrame(const Input& input){
		state.input = input;
		state.current.Reset();
		state.hot = state.hotToBe;
		state.hotToBe.Reset();

		if (state.hot.HasValue()) std::cout << "hot: \t" << state.hot << std::endl;
		if (state.hotToBe.HasValue()) std::cout << "hotToBe: \t" << state.hotToBe << std::endl;
		if (state.active.HasValue()) std::cout << "active: \t" << state.active << std::endl;

		// setup
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, input.screen.x, 0, input.screen.y, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// set origin to top left
		glTranslatef(0.5, input.screen.y, 0);
		glScalef(1, -1, 1);
		glLineWidth(3);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}
	void EndFrame(){
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	void BeginWindow(const char *name, Rect& window){
		//	+---------------------------------------+
		//  |				1                       |
		//  +-----------------------------------+---+
		//  |                                   |   |
		//  |               2                   | 3 |
		//  |                                   |   |
		//  +-----------------------------------+---+
		//  1 - Header, 2 - Body, 3 - Scroll
		state.current.NextDialog();
		float padding = state.style.WindowPadding;
		float textPadding = (padding - state.style.FontHeight) * 0.5f;
		Rect header(window.min, Vector2(window.max.x, window.max.y + padding));
		Rect body(
			Vector2(window.min.x, window.min.y + padding),
			Vector2(window.max.x - padding, window.max.y));
		Rect scroll(
			Vector2(window.max.x - padding, window.min.y + padding),
			Vector2(window.max.x, window.max.y - padding));
		if (IsActive()){
			if (CheckMouse(MouseButton::LEFT, MouseButtonState::UP)){
				ClearActive();
			}
			else {
				Rect dragArea(Vector2::ZERO, state.input.screen - Vector2(padding));
				window.MoveTo(dragArea.ClosestPoint(state.input.mouse - state.drag));
			}
		}
		if (IsHot() && CheckMouse(MouseButton::LEFT, MouseButtonState::DOWN)){
			state.active = state.current;
			state.drag = state.input.mouse - window.min;
		}
		if (body.Contains(state.input.mouse) || header.Contains(state.input.mouse)) {
			SetHot();
		}

		// Begin drawing
		glPushMatrix();
		Vector2 bottomRight = window.Size();
		glTranslatef(window.min.x, window.min.y, 0.f);
		// Header
		DrawTriangle(
			Vector2(0, padding),
			Vector2(padding),
			Vector2(padding, 0),
			state.style.Colors[Style::Palette::AccentHighlight]);
		DrawRect(
			Vector2(padding, 0),
			Vector2(bottomRight.x, padding),
			state.style.Colors[Style::Palette::AccentHighlight]);
		DrawString(padding + textPadding, padding - textPadding, state.style.Font, name, state.style.Colors[Style::Palette::Text]);
		// Body
		DrawRect(
			Vector2(0.f, padding),
			bottomRight,
			state.style.Colors[Style::Palette::Background], false);
	}

	void EndWindow(){
		glPopMatrix();
	}
	////////////////////////////////////////////////////////////////////
	// Helper implementations
	////////////////////////////////////////////////////////////////////

	bool IsHot(){ return state.hot.HasValue() && state.hot == state.current; }
	bool IsActive(){ return state.active.HasValue() && state.active == state.current; }
	void SetHot(){
		if (!state.active.HasValue() && state.hotToBe != state.current){
			state.hotToBe = state.current;
		}
	}
	void SetActive(){
		state.active = state.current;
	}
	void ClearActive(){
		state.active.Reset();
	}
	bool CheckMouse(MouseButton button, MouseButtonState buttonState){
		return state.input.button == button && state.input.buttonState == buttonState;
	}
}}}