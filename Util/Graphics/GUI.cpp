#include "UtilStdAfx.h"
#include "GUI.h"
#include "System/Memory.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Geometry.h"
#include "Drawing.h"
namespace TX { namespace UI { namespace GUI {
	class Window {
	public:
		const uint		id;
		DrawList		drawList;
		bool			accessed;
		bool			folded;
		float			contentHeight;
		float			scroll;
		void Reset(){ accessed = false; drawList.Clear(); }
		const Rect& GetClipRect(){ return drawList.clipRectStack.back(); }
		static uint GetID(const std::string& name){
			// sdbm
			uint hash = 0;
			for (const char& c : name){
				hash = c + (hash << 6) + (hash << 16) - hash;	// hash(i) = hash(i - 1) * 65599 + str[i];
			}
			return hash;
		}
	public:
		Window(uint id) :
			id(id),
			accessed(false),
			folded(false),
			scroll(0.f),
			contentHeight(1.f){}
	};

	// Ids that uniquely identify a widget inside a window
	struct Widget {
		Window*		window = nullptr;
		uint		itemId = 0;
		uint		index = 0;
		Widget() : window(nullptr), itemId(0), index(0){}
		Widget(const Widget& ot) : window(ot.window), itemId(ot.itemId), index(ot.index){}
		Widget& operator = (const Widget& ot){ window = ot.window; itemId = ot.itemId; index = ot.index; return *this; }
		bool operator == (const Widget& ot){ return window == ot.window && itemId == ot.itemId && index == ot.index; }
		bool operator != (const Widget& ot){ return window != ot.window || itemId != ot.itemId || index != ot.index; }
		void Reset(Window *w = nullptr){ window = w; itemId = index = 0; }
		bool HasValue(){ return window; }
	};
	std::ostream& operator << (std::ostream& os, const Widget& w)
	{
		return os << w.window->id << "-" << w.itemId << "-" << w.index;
	}

	struct State {
		Style					style;
		Input					input;
		Input*					inputPtr;
		Vec2					cursorBackup;	// used to disable cursor if it's outside the current window
		std::vector<Window*>	windows;
		Widget					current;
		Widget					hot;
		Widget					hotToBe;
		Widget					active;

		Vec2					initPos;	// initial widget position
		Vec2					widgetPos;	// current widget position
		const Color*			currColor;
		Vec2					drag;		// can be either mouse offset relative to the widget being dragged, or the total amount

		struct {
			std::string*	buffer;
			std::string		clipboard;
			GlyphPosMap		glyphPosMap;	// size = len + 1
			Widget			id;
			float			offset;
			uint			cursor;			// [0, len]
			uint			selectionBegin;

			bool Edit(char ch){
				bool changed = false;
				if (buffer){
					bool printable = std::isprint(ch);

					// editing characters
					if (printable){
						// remove selected characters
						DeleteSelection();
						if (printable){
							buffer->insert(buffer->begin() + cursor++, ch);
						}
						changed = true;
					}
				}
				return changed;
			}
			void Clear(){
				buffer = nullptr;
				id.Reset();
				cursor = 0;
				offset = 0.f;
				ClearSelection();
			}
			void Set(const Widget& wid, const FontMap& font, std::string& text){
				if (buffer != &text){
					Clear();
					buffer = &text;
					glyphPosMap.Recalculate(&font, text.data());		// precalculate glyph positions
				}
				id = wid;
			}
			int LocateIndex(float pos) { return glyphPosMap.GetIndex(pos + offset); }
			float LocatePos(int index) { return glyphPosMap.GetWidth(0, index) - offset; }
			bool HasSelection() { return selectionBegin != -1 && cursor != selectionBegin; }
			void SetCursor(int index){ cursor = Math::Clamp(index, 0, glyphPosMap.Size() - 1); }
			void Select(int begin, int end){ selectionBegin = Math::Clamp(begin, 0, glyphPosMap.Size()); SetCursor(end); }
			int SelectionLeft(){ return Math::Min(selectionBegin, cursor); }
			int SelectionRight(){ return Math::Max(selectionBegin, cursor); }
			int AdvanceWords(int steps){
				auto i = cursor;
				if (buffer->length() == 0 || steps == 0) return cursor;

				while (steps > 0){		// towards the right
					while (i < buffer->length() && !std::isspace((*buffer)[i])){
						i++;
					}
					while (i < buffer->length() && std::isspace((*buffer)[i])){
						i++;
					}
					steps--;
				}
				while (steps < 0){		// towards the left
					while (i > 0 && std::isspace((*buffer)[i - 1])){
						i--;
					}
					while (i > 0 && !std::isspace((*buffer)[i - 1])){
						i--;
					}
					steps++;
				}
				return i;
			}

			// Common actions
			void SelectAll(){ cursor = buffer->length(); Select(0, cursor); }
			void ClearSelection(){ selectionBegin = -1; }
			bool DeleteSelection(){
				if (HasSelection()){
					int pos = SelectionLeft();
					int len = SelectionRight() - pos;
					buffer->erase(pos, len);
					cursor = pos;
					ClearSelection();
					return true;
				}
				return false;
			}
			void CopySelection(){
				if (HasSelection()){
					int pos = SelectionLeft();
					int len = SelectionRight() - pos;
					clipboard.assign(*buffer, pos, len);
				}
			}
			bool CutSelection(){
				if (HasSelection()){
					int pos = SelectionLeft();
					int len = SelectionRight() - pos;
					clipboard.assign(*buffer, pos, len);
					DeleteSelection();
					return true;
				}
				return false;
			}
			bool Paste(){
				if (clipboard.length() > 0){
					for (auto ch : clipboard){
						Edit(ch);
					}
					return true;
				}
				return false;
			}
			bool Backspace(){
				if (DeleteSelection())
					return true;
				else if (cursor > 0){
					buffer->erase(--cursor, 1);
					return true;
				}
				return false;
			}
			bool Del(){
				if (DeleteSelection())
					return true;
				else if (cursor < buffer->length()){
					buffer->erase(cursor, 1);
					return true;
				}
				return false;
			}

			// Shifts text so that cursor is always visible
			void UpdateOffset(float width){
				width = Math::Max(0.f, width - 3.f);
				float cursorPos = LocatePos(cursor);
				if (cursorPos < 0){
					offset += cursorPos;
				}
				else if (cursorPos > width){
					offset += cursorPos - width;
				}
			}
		} textEdit;
		long long			cursorUpdatedTime;

		// OpenGL related
		GLuint				program;
		GLuint				vao;		// vertex array
		GLuint				vbo;		// vertex buffer
		GLuint				ebo;		// element buffer
		GLuint				vertShader;
		GLuint				fragShader;
		GLuint				locProj;		// proj: projection matrix
		GLuint				locTex;			// tex: texture
		GLuint				locPos;			// pos: vertex position
		GLuint				locUV;			// uv:	texture coordinate
		GLuint				locCol;			// col: color

		/// <summary>
		/// Get the pointer to the window, will create a new one if it didn't exist
		/// </summary>
		Window* GetWindow(const std::string& name){
			Window *result = nullptr;

			// search for existing windows
			uint id = Window::GetID(name);
			for (Window *w : windows) {
				if (w->id == id) {
					result = w;
					break;
				}
			}
			if (!result) {
				windows.push_back(new Window(id));
				result = windows.back();
			}
			result->accessed = true;
			return result;
		}
		Window* NextWindow(const std::string& name)					{ current.Reset(GetWindow(name)); return current.window; }
		const Rect& CurrentClipRect()								{ return current.window->GetClipRect(); }
		const Widget& NextItem()									{ current.itemId++; current.index = 0; return current; }
		const Widget& NextIndex()									{ current.index++; return current; }
		const void AdvanceLine(bool pad = false, int lines = 1)		{ widgetPos.y += style.LineHeight * lines; if (pad) widgetPos.y += style.WidgetPadding; }
		const int CompareDist(const Widget& w1, const Widget& w2) {
			if (w1.window == w2.window) return 0;
			if (!w1.window) return 1;
			if (!w2.window) return -1;
			for (Window *w : windows)
				if (w == w1.window || w == w2.window)
					return w == w1.window ? 1 : -1;
			assert(false);	// window does not exist
			return 0;
		}

		float CenterPadding(float containerSize, float elementSize){
			return (containerSize - elementSize) * 0.5f;
		}
	};
	static State G;
	static Vec2 InvalidCursor(Math::INF);

	////////////////////////////////////////////////////////////////////
	// Helper declaration
	////////////////////////////////////////////////////////////////////

	typedef std::string (Tagger)(const std::string& tagName, void *val);
	typedef void (StringCallback)(const std::string& str);

	/// <summary>
	/// The standard way of processing a widget.
	/// </summary>
	void WidgetLogic(std::function<bool()> shouldSetHot, std::function<void()> hotAction, std::function<void()> activeAction);
	bool IsHot();
	bool IsActive();
	bool IsEditing();
	void SetHot();
	void SetActive();
	void ClearActive();
	void WordWrap(const FontMap& font, const std::string& text, float maxWidth, StringCallback processLine);
	void ScrollBar(const Rect& hotArea, float areaHeight, float contentHeight, float& scroll);
	void Scroll(float& scroll, float step, float contentHeight);
	void WindowFolder();
	////////////////////////////////////////////////////////////////////
	// API implementation
	////////////////////////////////////////////////////////////////////

	Style& GetStyle(){ return G.style; }
	void Init(FontMap& font){
		G.style.Font = &font;
		G.style.Update();

		// add root window
		G.windows.push_back(new Window(0));

		const GLchar *vertShaderSrc = R"(
			#version 330
			uniform mat4 proj;
			in vec2 pos;
			in vec2 uv;
			in vec4 col;
			out vec2 fragUV;
			out vec4 fragCol;
			void main(){
				fragUV = uv;
				fragCol = col;
				gl_Position = proj * vec4(pos.xy,0,1);
			})";
		const GLchar *fragShaderSrc = R"(
			#version 330
			#define PRECISION 0.00001
			uniform sampler2D tex;
			in vec2 fragUV;
			in vec4 fragCol;
			out vec4 outCol;

			bool not_zero(float val){
				return step(-PRECISION, val) * (1.0 - step(PRECISION, val)) == 0.0;
			}

			void main(){
				outCol = fragCol;
				if (not_zero(fragUV.s) || not_zero(fragUV.t))
					outCol.w = texture(tex, fragUV.st).w;
			})";

		G.program = glCreateProgram();
		G.vertShader = glCreateShader(GL_VERTEX_SHADER);
		G.fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(G.vertShader, 1, &vertShaderSrc, 0);
		glShaderSource(G.fragShader, 1, &fragShaderSrc, 0);
		glCompileShader(G.vertShader);
		glCompileShader(G.fragShader);
		glAttachShader(G.program, G.vertShader);
		glAttachShader(G.program, G.fragShader);
		glLinkProgram(G.program);

		G.locProj = glGetUniformLocation(G.program, "proj");
		G.locTex = glGetUniformLocation(G.program, "tex");
		G.locPos = glGetAttribLocation(G.program, "pos");
		G.locUV = glGetAttribLocation(G.program, "uv");
		G.locCol = glGetAttribLocation(G.program, "col");

		glGenBuffers(1, &G.vbo);
		glGenBuffers(1, &G.ebo);

		glGenVertexArrays(1, &G.vao);
		glBindVertexArray(G.vao);
		glBindBuffer(GL_ARRAY_BUFFER, G.vbo);
		glEnableVertexAttribArray(G.locPos);
		glEnableVertexAttribArray(G.locUV);
		glEnableVertexAttribArray(G.locCol);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(G.locPos, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, pos));
		glVertexAttribPointer(G.locUV, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, uv));
		glVertexAttribPointer(G.locCol, 4, GL_FLOAT, GL_TRUE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, col));
#undef OFFSETOF

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void Shutdown(){
		for (Window *w : G.windows) MemDelete(w);

		if (G.vao) glDeleteVertexArrays(1, &G.vao);
		if (G.vbo) glDeleteBuffers(1, &G.vbo);
		if (G.ebo) glDeleteBuffers(1, &G.ebo);
		G.vao = G.vbo = G.ebo = 0;

		glDeleteProgram(G.program);
		glDeleteShader(G.vertShader);
		glDeleteShader(G.fragShader);
		G.program = G.vertShader = G.fragShader = 0;
	}

	void BeginFrame(Input& input){
		glPushAttrib(GL_ENABLE_BIT);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		G.inputPtr = &input;
		G.input = input;
		G.cursorBackup = InvalidCursor;
		G.current.Reset();
		G.hot = G.hotToBe;
		G.hotToBe.Reset();
		for (Window *w : G.windows) w->Reset();
	}
	void EndFrame(){
		G.current.Reset(G.windows[0]);
		G.windows[0]->drawList.PushClipRectFullScreen();
		WindowFolder();

		// consume mouse actions if it is hovering on any window
		if (G.hot.HasValue()) {
			G.inputPtr->ClearButton();
			G.inputPtr->ClearScroll();
		}
		// focus on the window where the active widget is located
		if (G.active.HasValue()){
			Window *window = G.active.window;
			// move the window to the end (skip root window)
			if (window != G.windows.back() && window->id != 0) {
				for (uint i = 1; i < G.windows.size(); i++) {
					if (G.windows[i] == window) {
						G.windows.erase(G.windows.begin() + i);
						break;
					}
				}
				G.windows.push_back(window);
			}
		}
		// delete windows we didn't touch in this frame (skip root window)
		G.windows.erase(
			std::remove_if(
				G.windows.begin() + 1, G.windows.end(),
				[](Window *w) {bool die = !w->accessed; if (die) MemDelete(w); return die; }),
			G.windows.end());

		// ============================================================
		// backup program & texture
		GLint lastProgram, lastTexture;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);

		// setup
		auto& w = G.input.windowSize.x;
		auto& h = G.input.windowSize.y;
		const Matrix4x4 orthoProjection(
			2.f/w,	0.f,	0.f,	0.f,
			0.f,	2.f/-h,	0.f,	0.f,
			0.f,	0.f,	-1.f,	0.f,
			-1.f,	1.f,	0.f,	1.f);

		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glActiveTexture(GL_TEXTURE0);

		// Render windows
		glUseProgram(G.program);
		glUniform1i(G.locTex, 0);
		glUniformMatrix4fv(G.locProj, 1, GL_FALSE, &orthoProjection[0][0]);
		glBindVertexArray(G.vao);
		glBindTexture(GL_TEXTURE_2D, G.style.Font->TexID());
		for (Window *w : G.windows){
			const DrawIdx *idxBufOffset = 0;
			DrawList& drawList = w->drawList;
			if (!w->folded && drawList.cmdBuf.size() > 0){
				glBindBuffer(GL_ARRAY_BUFFER, G.vbo);
				glBufferData(GL_ARRAY_BUFFER,
					(GLsizeiptr)drawList.vtxBuf.size() * sizeof(DrawVert),
					(GLvoid*)drawList.vtxBuf.data(),
					GL_STREAM_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G.ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					(GLsizeiptr)drawList.idxBuf.size() * sizeof(DrawIdx),
					(GLvoid*)drawList.idxBuf.data(),
					GL_STREAM_DRAW);
				for (const DrawCmd& cmd : drawList.cmdBuf){
					glScissor((int)cmd.clipRect.min.x, (int)(h - cmd.clipRect.max.y), (int)cmd.clipRect.Width(), (int)cmd.clipRect.Height());
					glDrawElements(GL_TRIANGLES,
						(GLsizei)cmd.idxCount,
						GL_UNSIGNED_SHORT,
						idxBufOffset);
					idxBufOffset += cmd.idxCount;
				}
			}
		}

		// restore program & settings
		glUseProgram(lastProgram);
		glPopClientAttrib();
		glPopAttrib();
	}
	//	+---------------------------------------+
	//  |                 1                     |
	//  +---------------------------------------+
	//  |                                       |
	//  |                 2                     |
	//  |                                       |
	//  +-----------------------------------+---+
	//  |                 3                 | 4 |
	//  +-----------------------------------+---+
	//  1 - Header, 2 - Body(Scrollable), 3 - Bottom, 4 - Resize handle
	//
	// - Middle-click on anywhere to minimize/fold the window
	bool BeginWindow(const std::string& name, Rect& rect){
		Window *W = G.NextWindow(name);
		if (W->folded)
			return false;

		W->drawList.PushClipRect(rect);

		float padding = G.style.WindowPadding;
		float textPadding = G.style.TextPaddingY;
		Rect header(rect.min, Vec2(rect.max.x, rect.min.y + padding));
		Rect body(
			Vec2(rect.min.x, rect.min.y + padding),
			Vec2(rect.max.x, rect.max.y - padding));
		Rect bottom(
			Vec2(rect.min.x, rect.max.y - padding),
			Vec2(rect.max.x - padding, rect.max.y));

		Rect dragArea(Vec2::ZERO, G.input.windowSize - Vec2(padding));
		#pragma region window logic
		// restrict window position to inside the actual window
		if (G.input.windowChanged) {
			rect.MoveTo(dragArea.ClosestPoint(rect.min));
		}
		if (IsActive()){
			if (G.input(MouseButton::LEFT, MouseButtonState::UP)){
				ClearActive();
			}
			else {
				rect.MoveTo(dragArea.ClosestPoint(G.input.cursor - G.drag));
			}
		}
		if (body.Contains(G.input.cursor) || header.Contains(G.input.cursor) || bottom.Contains(G.input.cursor)) {
			SetHot();
		}
		if (IsHot()){
			if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)){
				SetActive();
				G.drag = G.input.cursor - rect.min;
			}
			else if (G.input(MouseButton::MIDDLE, MouseButtonState::DOWN)) {
				W->folded = true;
			}
			else if (G.input.scroll != 0.f){
				Scroll(W->scroll, -G.input.scroll, W->contentHeight);
			}
		}

		#pragma endregion
		#pragma region window rendering

		// Header
		W->drawList.AddTriangle(
			Vec2(rect.min.x, rect.min.y + padding),
			rect.min + Vec2(padding),
			Vec2(rect.min.x + padding, rect.min.y),
			G.style.Colors[Style::Palette::Accent]);
		W->drawList.AddRect(
			Vec2(rect.min.x + padding, rect.min.y),
			header.max,
			G.style.Colors[Style::Palette::Accent]);
		W->drawList.AddText(
			rect.min.x + padding + textPadding,
			rect.min.y + padding - textPadding,
			G.style.Font,
			name,
			G.style.Colors[Style::Palette::Text]);
		// Body
		W->drawList.AddRect(body.min, body.max, G.style.Colors[Style::Palette::Background]);
		// Bottom
		W->drawList.AddRect(bottom.min, bottom.max, G.style.Colors[Style::Palette::Background]);
		#pragma endregion

		// Resize handle
		G.NextItem();
		Rect resize(rect.max - padding, rect.max);
		#pragma region resize handle logic
		Color *resizeColor = &G.style.Colors[Style::Palette::Accent];
		WidgetLogic(
			[&] { return resize.Contains(G.input.cursor); },
			[&] {
				resizeColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					SetActive();
					G.drag = G.input.cursor - rect.max;
				}
			},
			[&] {
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					resizeColor = &G.style.Colors[Style::Palette::AccentHighlight];
					ClearActive();
				}
				else {
					resizeColor = &G.style.Colors[Style::Palette::AccentActive];
					rect.max.x = Math::Max(rect.min.x + 10 * padding, G.input.cursor.x - G.drag.x);
					rect.max.y = Math::Max(rect.min.y + 5 * padding, G.input.cursor.y - G.drag.y);
				}
			});

		#pragma endregion
		#pragma region resize handle rendering
		W->drawList.AddTriangle(body.max, bottom.TR(), bottom.max, *resizeColor, true);
		#pragma endregion

		// Scroll bar
		Rect scrollBarArea(
			body.max.x - padding,
			body.min.y,
			body.max.x,
			body.max.y);
		Rect scrollRect = Rect(
			body.min.x + padding,
			body.min.y + G.style.WidgetPadding,
			body.max.x - padding,
			body.max.y);
		float scrollRectHeight = scrollRect.Height();
		float scrollRectCenterY = scrollRect.min.y + scrollRectHeight * 0.5f;
		ScrollBar(scrollBarArea, scrollRectHeight, W->contentHeight, W->scroll);

		float contentOffset = (W->contentHeight > scrollRectHeight) ? W->scroll * (W->contentHeight - scrollRectHeight) : 0.f;
		G.initPos = G.widgetPos = Vec2(scrollRect.min.x, scrollRect.min.y - contentOffset);

		// Update clip rect
		W->drawList.PushClipRect(scrollRect);

		// disable cursor/widget interaction if it's outside the window
		if (!IsActive() && !rect.Contains(G.input.cursor)) {
			G.cursorBackup = G.input.cursor;
			G.input.cursor = InvalidCursor;
		}

		return true;
	}

	void EndWindow(){
		G.current.window->contentHeight = G.widgetPos.y - G.initPos.y;

		if (G.cursorBackup != InvalidCursor) {
			G.input.cursor = G.cursorBackup;
		}
	}

	void Divider(){
		Vec2 points[2];
		const Rect& clipRect = G.CurrentClipRect();
		points[0] = G.widgetPos;
		points[1].x = clipRect.max.x;
		points[1].y = G.widgetPos.y;
		#pragma region rendering
		G.current.window->drawList.AddPolyLine(
			points, 2,
			G.style.Colors[Style::Palette::Hint],
			false, 1.f);
		#pragma endregion
		G.widgetPos.y += G.style.WidgetPadding;
	}
	void Text(const std::string& text, bool isHint){
		G.NextItem();
		G.currColor = &G.style.Colors[isHint ? Style::Palette::Hint : Style::Palette::Text];

		#pragma region rendering
		WordWrap(*G.style.Font, text, G.CurrentClipRect().Width(), [](const std::string& line){
			G.AdvanceLine();
			G.current.window->drawList.AddText(
				G.widgetPos.x,
				G.widgetPos.y - G.style.TextPaddingY,
				G.style.Font,
				line.data(), *G.currColor);
		});
		G.AdvanceLine(true, 0);
		#pragma endregion
	}
	bool Button(const std::string& name, bool enabled){
		G.NextItem(); bool clicked = false;

		Color *bgColor = &G.style.Colors[Style::Palette::Accent];
		float textWidth = G.style.Font->GetWidth(name);
		Rect button(G.widgetPos, G.widgetPos + Vec2(textWidth + 2.f * G.style.TextPaddingX, G.style.LineHeight));
		bool hovering = button.Contains(G.input.cursor);
		#pragma region logic
		WidgetLogic(
			[&hovering] { return hovering; },
			[&] {
				bgColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					SetActive();
				}
			},
			[&] {
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					bgColor = &G.style.Colors[Style::Palette::AccentHighlight];
					ClearActive();
					if (hovering) {
						clicked = true;
					}
				}
				else {
					bgColor = &G.style.Colors[Style::Palette::AccentActive];
				}
			});
		#pragma endregion
		#pragma region rendering
		G.current.window->drawList.AddRect(button.min, button.max, *bgColor, true);
		G.current.window->drawList.AddText(
			button.min.x + G.style.TextPaddingX,
			button.max.y - G.style.TextPaddingY,
			G.style.Font,
			name,
			G.style.Colors[Style::Palette::Text]);
		G.AdvanceLine(true);
		#pragma endregion
		return clicked;
	}
	template <typename T>
	bool Slider(const std::string& name, const Vec2& pos, float width, T *val, T min, T max, T step, Tagger getTag){
		G.NextItem(); bool changed = false;

		Color *sliderColor = &G.style.Colors[Style::Palette::Accent];
		Color *trackColor = sliderColor;
		float sliderSize = G.style.HalfLineHeight();
		float halfSliderSize = sliderSize / 2.f;

		Rect hotArea(
			pos.x, pos.y + sliderSize,
			pos.x + width, pos.y + G.style.LineHeight);
		Vec2 slider(pos.x + halfSliderSize, pos.y + G.style.LineHeight * 0.75f);
		float length = hotArea.Width() - sliderSize;
		#pragma region logic
		WidgetLogic(
			[&] { return hotArea.Contains(G.input.cursor); },
			[&] {
				trackColor = sliderColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN))
					SetActive();
			},
			[&] {
				trackColor = &G.style.Colors[Style::Palette::AccentHighlight];
				T newVal = T(Math::Lerp(Math::Clamp(G.input.cursor.x - slider.x, 0.f, length) / length, float(min), float(max)));
				if (step > Math::EPSILON) {
					newVal = Math::Clamp(Math::Round(float(newVal - min) / step) * step + min, min, max);
				}
				if (newVal != *val) {
					*val = newVal;
					changed = true;
				}
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					sliderColor = &G.style.Colors[Style::Palette::AccentHighlight];
					ClearActive();
				}
				else {
					sliderColor = &G.style.Colors[Style::Palette::AccentActive];
				}
			});
		float offset = Math::Clamp(float(*val - min) / (max - min) * length, 0.f, length);
		#pragma endregion
		#pragma region rendering
		// Text
		std::string& tag = getTag(name, val);
		if (tag.length() > 0)
			G.current.window->drawList.AddText(
				hotArea.min.x, hotArea.min.y - G.style.TextPaddingY,
				G.style.Font, getTag(name, val).data(),
				G.style.Colors[Style::Palette::Text]);
		// Slider
		Vec2 trackLine[2] = { slider, slider + Vec2(length, 0.f) };
		slider.x += offset;
		G.current.window->drawList.AddPolyLine(trackLine, 2, *trackColor, false, G.style.StrokeWidth);
		G.current.window->drawList.AddRect(
			slider - halfSliderSize,
			slider + halfSliderSize,
			*sliderColor,
			true);
		#pragma endregion
		return changed;
	}
	bool FloatSlider(const std::string& name, float& val, float min, float max, float step){
		G.AdvanceLine(true, 0);	// extra padding fix
		bool changed = Slider<float>(name, G.widgetPos, G.CurrentClipRect().Width(), &val, min, max, step, [](const std::string& name, void *v){
			std::ostringstream text;
			text << name << ":  " << std::setprecision(4) << std::fixed << *((float *)v);
			return text.str();
		});
		G.AdvanceLine(true);
		return changed;
	}
	bool IntSlider(const std::string& name, int& val, int min, int max, int step){
		G.AdvanceLine(true, 0);	// extra padding fix
		bool changed = Slider<int>(name, G.widgetPos, G.CurrentClipRect().Width(), &val, min, max, step, [](const std::string& name, void *v){
			std::ostringstream text;
			text << name << ":  " << *((int *)v);
			return text.str();
		});
		G.AdvanceLine(true);
		return changed;
	}
	bool ColorSlider(const std::string& name, Color &val, Color::Channel channel){
		G.NextItem(); bool changed = false;
		G.AdvanceLine(true, 0);	// extra padding fix

		auto updateColor = [&val, &changed](const Color& newVal){
			if (newVal != val){
				changed = true;
				val = newVal;
			}
		};
		int sliderCount = static_cast<int>(channel);
		Rect sampleArea(G.widgetPos, G.widgetPos + Vec2(G.style.HalfLineHeight(), G.style.LineHeight));
		Vec2 sliderPos(sampleArea.max.x + G.style.WidgetPadding, G.widgetPos.y);
		float sliderWidth = (G.CurrentClipRect().max.x - sliderPos.x - (sliderCount - 1) * G.style.WidgetPadding) / sliderCount;

		#pragma region logic
		Color temp = val.Convert(channel);
		updateColor(temp);
		if (IsActive()) {
			// picking pixel color
			glReadPixels(int(G.input.cursor.x), int(G.input.cursor.y), 1, 1, GL_RGB, GL_FLOAT, &temp);
			std::cout << temp << std::endl;
			updateColor(temp.Convert(channel, false));
			if (G.input(MouseButton::LEFT, MouseButtonState::UP))
				ClearActive();
		}
		if (sampleArea.Contains(G.input.cursor)){
			SetHot();
		}
		if (IsHot() && !IsActive()){
			if (G.input(MouseButton::LEFT, MouseButtonState::DOWN))
				SetActive();
		}
		#pragma endregion
		#pragma region rendering color sample
		// background
		G.current.window->drawList.AddRect(sampleArea.min, sampleArea.max, Color::WHITE);
		// sample
		G.current.window->drawList.AddRect(sampleArea.min, sampleArea.max, val);
		#pragma endregion

		#pragma region rendering slider(s)
		for (int i = 0; i < sliderCount; i++){
			const std::string& tag = i == 0 ? name : "";
			changed |= Slider<float>(tag, sliderPos, sliderWidth, &val[i], 0.f, 1.f, 0.f, [](const std::string& name, void *v){
				return std::string(name);
			});
			sliderPos.x += sliderWidth + G.style.WidgetPadding;
		}

		G.AdvanceLine(true);
		#pragma endregion

		return changed;
	}
	bool RadioButton(const std::string& name, int& val, int itemVal){
		G.NextItem(); bool changed = false;

		Color *ringColor = &G.style.Colors[Style::Palette::Accent];
		Rect hotArea(G.widgetPos, G.widgetPos + Vec2(G.style.LineHeight));
		#pragma region logic
		WidgetLogic(
			[&] { return hotArea.Contains(G.input.cursor); },
			[&] {
				ringColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					SetActive();
				}
			},
			[&] {
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					ringColor = &G.style.Colors[Style::Palette::AccentHighlight];
					if (val != itemVal) {
						val = itemVal;
						changed = true;
					}
					ClearActive();
				}
				else {
					ringColor = &G.style.Colors[Style::Palette::AccentActive];
				}
			});
		#pragma endregion
		#pragma region rendering
		Vec2 center = hotArea.Center();
		G.current.window->drawList.AddCircle(
			center,
			G.style.FormWidgetRadius,
			*ringColor, true);
		if (val == itemVal){
			G.current.window->drawList.AddCircle(
				center,
				G.style.FormWidgetSelectedRadius,
				G.style.Colors[Style::Palette::AccentActive],
				true);
		}
		G.current.window->drawList.AddText(
			hotArea.max.x + G.style.TextPaddingX,
			hotArea.max.y - G.style.TextPaddingY,
			G.style.Font,
			name,
			G.style.Colors[Style::Palette::Text]);
		#pragma endregion
		G.AdvanceLine(true);
		return changed;
	}
	bool CheckBox(const std::string& name, bool& val){
		G.NextItem(); bool changed = false;

		Color *boxColor = &G.style.Colors[Style::Palette::Accent];
		Rect hotArea(G.widgetPos, G.widgetPos + Vec2(G.style.LineHeight));
		#pragma region logic
		WidgetLogic(
			[&] {return hotArea.Contains(G.input.cursor); },
			[&] {
				boxColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					SetActive();
				}
			},
			[&] {
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					boxColor = &G.style.Colors[Style::Palette::AccentHighlight];
					ClearActive();
					val = !val;
					changed = true;
				}
				else {
					boxColor = &G.style.Colors[Style::Palette::AccentActive];
				}
			});
		#pragma endregion
		#pragma region rendering
		Vec2 center = hotArea.Center();
		G.current.window->drawList.AddRect(
			center - G.style.FormWidgetRadius,
			center + G.style.FormWidgetRadius,
			*boxColor, true);
		if (val){
			G.current.window->drawList.AddRect(
				center - G.style.FormWidgetSelectedRadius,
				center + G.style.FormWidgetSelectedRadius,
				G.style.Colors[Style::Palette::AccentActive],
				true);
		}
		G.current.window->drawList.AddText(
			hotArea.max.x + G.style.TextPaddingX,
			hotArea.max.y - G.style.TextPaddingY,
			G.style.Font,
			name,
			G.style.Colors[Style::Palette::Text]);
		G.AdvanceLine(true);
		#pragma endregion
		return changed;
	}
	void ProgressBar(const std::string& name, const float& zero2One){
		G.NextItem();
		G.AdvanceLine();

		float padding = G.style.HalfLineHeight() / 2.f;
		float fullLength = G.CurrentClipRect().max.x - G.widgetPos.x - 2.f * padding;
		float barLength = fullLength * Math::Clamp(zero2One, 0.f, 1.f);

		#pragma region rendering
		// Text
		G.current.window->drawList.AddText(
			G.widgetPos.x, G.widgetPos.y + G.style.HalfLineHeight() - G.style.TextPaddingY,
			G.style.Font, name,
			G.style.Colors[Style::Palette::Text]);
		// Progress bar
		Vec2 line[3];
		line[0] = Vec2(G.widgetPos.x + padding, G.widgetPos.y + G.style.HalfLineHeight() + padding);
		line[1] = line[0] + Vec2(barLength, 0.f);
		line[2] = line[0] + Vec2(fullLength, 0.f);
		if (line[0].x < line[1].x)
			G.current.window->drawList.AddPolyLine(line, 2, G.style.Colors[Style::Palette::AccentActive], false, G.style.StrokeWidth);
		if (line[1].x < line[2].x)
			G.current.window->drawList.AddPolyLine(line + 1, 2, G.style.Colors[Style::Palette::Accent], false, G.style.StrokeWidth);

		G.AdvanceLine(true);
		#pragma endregion
	}
	bool TextField(const std::string& name, std::string& text, bool selectAllOnActive){
		G.NextItem(); bool changed = false; bool cursorUpdated = false;

		G.AdvanceLine();
		Vec2 tagPos = G.widgetPos;
		G.widgetPos.y += G.style.TextPaddingY;
		Rect bgArea(G.widgetPos, Vec2(G.CurrentClipRect().max.x, G.widgetPos.y + G.style.LineHeight));
		Rect textArea(bgArea);
		textArea.Shrink(Vec2(G.style.TextPaddingY, 0.f));

		#pragma region logic
		WidgetLogic(
			[&] { return textArea.Contains(G.input.cursor); },
			[&] {
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					bool enteringFocus = !IsEditing();
					G.textEdit.Set(G.current, *G.style.Font, text);

					if (enteringFocus) {
						if (selectAllOnActive) {
							G.textEdit.SelectAll();
						}
						else {
							G.textEdit.SetCursor(G.textEdit.LocateIndex(G.input.cursor.x - textArea.min.x));
							G.textEdit.Select(G.textEdit.cursor, G.textEdit.cursor);
						}
						G.textEdit.UpdateOffset(textArea.Width());
						// do not start mouse selection this time
					}
					else {
						// start selection
						G.textEdit.SetCursor(G.textEdit.LocateIndex(G.input.cursor.x - textArea.min.x));
						G.textEdit.Select(G.textEdit.cursor, G.textEdit.cursor);
						cursorUpdated = true;
						SetActive();
					}
				}
			},
			[&] {
				// in the middle of selection by mouse
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					ClearActive();
				}
				else {
					G.textEdit.SetCursor(G.textEdit.LocateIndex(G.input.cursor.x - textArea.min.x));
					G.textEdit.UpdateOffset(textArea.Width());
				}
			});
		if (G.input(MouseButton::LEFT, MouseButtonState::DOWN) && !IsHot()) {
			G.textEdit.Clear();
		}

		// editing text
		if (IsEditing()){
			bool ignoreMouse = false;
			// Special keys
			if (G.input.HasKeyCode() && (G.input.keyState == KeyState::DOWN || G.input.keyState == KeyState::HOLD)){
				if (G.input.key == KeyCode::LEFT || G.input.key == KeyCode::RIGHT){
					ignoreMouse = true;
					bool left = G.input.key == KeyCode::LEFT;
					int step = (left ? -1 : 1);

					if (G.textEdit.HasSelection()){
						if (G.input.mods(Modifiers::SHIFT)){
							if (G.input.mods(Modifiers::CTRL))
								G.textEdit.SetCursor(G.textEdit.AdvanceWords(step));
							else
								G.textEdit.SetCursor(G.textEdit.cursor + step);
						}
						else {
							G.textEdit.SetCursor(left ? G.textEdit.SelectionLeft() : G.textEdit.SelectionRight());
							G.textEdit.ClearSelection();
						}
					}
					else {
						if (G.input.mods(Modifiers::SHIFT)){
							if (G.input.mods(Modifiers::CTRL))
								G.textEdit.Select(G.textEdit.cursor, G.textEdit.AdvanceWords(step));
							else
								G.textEdit.Select(G.textEdit.cursor, G.textEdit.cursor + step);
						}
						else {
							G.textEdit.ClearSelection();
							if (G.input.mods(Modifiers::CTRL))
								G.textEdit.SetCursor(G.textEdit.AdvanceWords(step));
							else
								G.textEdit.SetCursor(G.textEdit.cursor + step);
						}
					}
					cursorUpdated = true;
				}

				// keys that work only with modifier
				if (G.input.mods(Modifiers::CTRL)){
					switch (G.input.key){
					case KeyCode::A:
						G.textEdit.SelectAll();
						cursorUpdated = true;
						break;
					case KeyCode::C:
						G.textEdit.CopySelection();
						break;
					case KeyCode::V:
						changed |= G.textEdit.Paste();
						break;
					case KeyCode::X:
						changed |= G.textEdit.CutSelection();
						break;
					}
				}

				// keys that work regardless of the modifier
				switch (G.input.key){
				case KeyCode::DELETE:
					if (!G.textEdit.HasSelection() && G.input.mods(Modifiers::CTRL))
						G.textEdit.Select(G.textEdit.cursor, G.textEdit.AdvanceWords(1));
					changed |= G.textEdit.Del();
					break;
				case KeyCode::BACKSPACE:
					if (!G.textEdit.HasSelection() && G.input.mods(Modifiers::CTRL))
						G.textEdit.Select(G.textEdit.cursor, G.textEdit.AdvanceWords(-1));
					changed |= G.textEdit.Backspace();
				}
			}
			// text input
			if (G.input.HasText())
				changed |= G.textEdit.Edit(G.input.text);

			if (ignoreMouse){
				ClearActive();
			}
			cursorUpdated |= changed;
			G.textEdit.UpdateOffset(textArea.Width());
		}

		// update edit timestamp
		auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (cursorUpdated)
			G.cursorUpdatedTime = now;
		auto timeSinceLastCursorUpdate = now - G.cursorUpdatedTime;
		#pragma endregion

		#pragma region rendering text
		// field name
		G.current.window->drawList.AddText(tagPos.x, tagPos.y, G.style.Font, name, G.style.Colors[Style::Palette::Text]);
		// field background
		G.current.window->drawList.AddRect(bgArea.min, bgArea.max, G.style.Colors[Style::Palette::Hint], true);
		G.current.window->drawList.PushClipRect(textArea);
		if (IsEditing()){
			// selection
			if (G.textEdit.HasSelection()){
				G.current.window->drawList.AddRect(
					Vec2(
						textArea.min.x + G.textEdit.LocatePos(G.textEdit.SelectionLeft()),
						textArea.min.y + G.style.TextPaddingY / 2
					),
					Vec2(
						textArea.min.x + G.textEdit.LocatePos(G.textEdit.SelectionRight()),
						textArea.max.y - G.style.TextPaddingY / 2
					),
					G.style.Colors[Style::Palette::Accent],
					true);
			}

			// offset text
			G.current.window->drawList.AddText(
				textArea.min.x - G.textEdit.offset,
				textArea.max.y - G.style.TextPaddingY,
				G.style.Font,
				text.data(),
				G.style.Colors[Style::Palette::Text],
				&G.textEdit.glyphPosMap);
			// blinking cursor
			if (timeSinceLastCursorUpdate % 1000 / 500 == 0){
				float cursorPos = textArea.min.x + G.textEdit.LocatePos(G.textEdit.cursor);
				Vec2 cursorLine[2] = {
					Vec2(cursorPos, textArea.min.y + G.style.TextPaddingY / 2),
					Vec2(cursorPos, textArea.max.y - G.style.TextPaddingY / 2)
				};
				G.current.window->drawList.AddPolyLine(cursorLine, 2, G.style.Colors[Style::Palette::Foreground], false, 1.f);
			}
		}
		else {
			// text
			G.current.window->drawList.AddText(
				textArea.min.x,
				textArea.max.y - G.style.TextPaddingY,
				G.style.Font,
				text.data(),
				G.style.Colors[Style::Palette::Text]);
		}
		G.current.window->drawList.PopClipRect();
		G.AdvanceLine(true);
		#pragma endregion

		return changed;
	}

	////////////////////////////////////////////////////////////////////
	// Helper implementations
	////////////////////////////////////////////////////////////////////

	void WidgetLogic(std::function<bool()> shouldSetHot, std::function<void()> hotAction, std::function<void()> activeAction) {
		if (shouldSetHot()) {
			SetHot();
		}
		if (IsHot()) {
			hotAction();
		}
		if (IsActive()) {
			activeAction();
		}
		//if (shouldSetHot()) {
		//	SetHot();
		//}
		// have just left active state
		//if (!IsActive()) {
		//	G.hot = G.current;
		//}
	}

	bool IsHot(){ return G.hot.HasValue() && G.hot == G.current; }
	bool IsActive(){ return G.active.HasValue() && G.active == G.current; }
	bool IsEditing(){ return G.textEdit.id.HasValue() && G.textEdit.id == G.current; }
	void SetHot(){
		if (!G.active.HasValue() &&
			G.hotToBe != G.current &&
			G.CompareDist(G.hotToBe, G.current) >= 0) {
			G.hotToBe = G.current;
		}
	}
	void SetActive(){
		G.active = G.current;
	}
	void ClearActive(){
		if (IsActive()) {
			G.active.Reset();
			SetHot();
		}
	}

	void WordWrap(const FontMap& font, const std::string& text, float maxWidth, StringCallback processLine){
		std::istringstream words(text);
		std::ostringstream line;
		std::string word;
		float wordWidth, spaceLeft = maxWidth;
		const float spaceWidth = font.GetWidth(" ");
		while (words >> word){
			wordWidth = font.GetWidth(word.data());
			while (word.length() > 0){
				if (spaceLeft < wordWidth + spaceWidth){
					if (spaceLeft == maxWidth){
						// break down the word without hyphenation
						int length = word.length();
						float segWidth = 0.f;
						int charCount = 0;
						float charWidth = font.GetWidth(word[charCount]);
						while (segWidth + charWidth < maxWidth && charCount < length){
							segWidth += charWidth;
							charWidth = font.GetWidth(word[++charCount]);
						}
						processLine(charCount != length ? std::string(word.begin(), word.begin() + charCount) : word);
						wordWidth -= segWidth;
						word.erase(0, charCount);
					}
					else {
						processLine(line.str());
					}
					line.clear();
					line.str("");
					spaceLeft = maxWidth;
				}
				else {
					line << word << ' ';
					word.clear();
					spaceLeft -= wordWidth + spaceWidth;
				}
			}
		}
		if (line.tellp() > 0){
			processLine(line.str());
		}
	}
	void ScrollBar(const Rect& hotArea, float areaHeight, float contentHeight, float& scroll){
		G.NextItem();

		Color* barColor = &G.style.Colors[Style::Palette::Accent];
		float areaWidth = hotArea.Width();
		float padding = areaWidth * 0.35f;
		float ratio = areaHeight / contentHeight;
		float barHalfHeight = Math::Max(G.style.WidgetPadding, (hotArea.Height() - 2 * padding) * ratio) * 0.5f;
		float rangeStart = hotArea.min.y + padding + barHalfHeight;
		float rangeEnd = hotArea.max.y - padding - barHalfHeight;

		if (rangeStart >= rangeEnd)
			return;

		float barCenterY = Math::Lerp(scroll, rangeStart, rangeEnd);

		#pragma region logic
		WidgetLogic(
			[&] {return hotArea.Contains(G.input.cursor); },
			[&] {
				bool hovering = Math::InBounds(
					G.input.cursor.y,
					barCenterY - barHalfHeight,
					barCenterY + barHalfHeight);
				if (hovering)
					barColor = &G.style.Colors[Style::Palette::AccentHighlight];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					SetActive();
					G.drag.y = hovering ? G.input.cursor.y - barCenterY : 0.f;
				}
				else if (G.input.scroll != 0.f) {
					Scroll(scroll, -G.input.scroll, contentHeight);
				}
			},
			[&] {
				barCenterY = Math::Clamp(G.input.cursor.y - G.drag.y, rangeStart, rangeEnd);
				scroll = Math::InvLerp(barCenterY, rangeStart, rangeEnd);
				if (G.input(MouseButton::LEFT, MouseButtonState::UP)) {
					ClearActive();
					barColor = &G.style.Colors[Style::Palette::AccentHighlight];
				}
				else {
					barColor = &G.style.Colors[Style::Palette::AccentActive];
				}
			});
		#pragma endregion

		#pragma region rendering

		Rect bar(
			hotArea.min.x + padding,
			barCenterY - barHalfHeight,
			hotArea.max.x - padding,
			barCenterY + barHalfHeight);
		G.current.window->drawList.AddRect(bar.min, bar.max, *barColor);
		#pragma endregion
	}
	void Scroll(float& scroll, float step, float contentHeight){
		scroll = Math::Clamp(scroll + step * G.style.ScrollSpeed / contentHeight, 0.f, 1.f);
	}
	void WindowFolder() {
		G.NextItem();

		bool hasFoldedWindow = false;
		for (Window *w : G.windows)
			if (w->folded)
				hasFoldedWindow = true;

		if (!hasFoldedWindow)
			return;

		Rect folderSwitch(0, 0, G.style.WindowPadding, G.style.WindowPadding);
		bool hovering = folderSwitch.Contains(G.input.cursor);
		Color *color = &G.style.Colors[Style::Palette::AccentHighlight];
		#pragma region logic
		WidgetLogic(
			[&hovering] { return hovering; },
			[&] {
				color = &G.style.Colors[Style::Palette::AccentActive];
				if (G.input(MouseButton::LEFT, MouseButtonState::DOWN)) {
					// clicked: unfold all windows
					if (hovering) {
						for (Window *w : G.windows)
							w->folded = false;
					}
				}
			},
			[&] {});
		#pragma endregion
		#pragma region rendering
		G.current.window->drawList.AddTriangle(
			folderSwitch.min,
			folderSwitch.BL(),
			folderSwitch.TR(),
			*color);
		#pragma endregion
	}
}}}
