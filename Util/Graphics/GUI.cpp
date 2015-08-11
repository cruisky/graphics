#include "Util.h"
#include "GUI.h"
#include "System/Memory.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Geometry.h"
#include "Drawing.h"

using std::vector;

namespace TX { namespace UI { namespace GUI {
	class Window {
	public:
		const uint32	id;
		DrawList		drawList;
		bool			accessed;
		void Reset(){ accessed = false; drawList.Clear(); }
		static uint32 GetID(const char *name){
			// sdbm
			uint32 hash = 0;
			char c;
			while (c = *name++){
				hash = c + (hash << 6) + (hash << 16) - hash;	// hash(i) = hash(i - 1) * 65599 + str[i]; 
			}
			return hash;
		}
	public:
		Window(uint32 id) : id(id), accessed(false){}
	};

	// Ids that uniquely identify a widget inside a window
	struct Widget {
		Window*		window = nullptr;
		uint32		itemId = 0;
		uint32		index = 0;
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
		return os << w.window << "-" << w.itemId << "-" << w.index;
	}

	struct State {
		Style				style;
		Input				input;
		Widget				current;
		Widget				hot;
		Widget				hotToBe;
		Widget				active;
		vector<Window*>		windows;

		Vector2				drag;		// can be either mouse offset relative to the widget being dragged, or the total amount

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
		
		// Get the pointer to the window, will create a new one if it didn't exist
		Window* GetWindow(const char *name){
			Window *result = nullptr;
			if (name){
				// search for existing windows
				uint32 id = Window::GetID(name);
				for (Window *w : windows){
					if (w->id == id){
						result = w;
						break;
					}
				}
				if (!result){
					windows.emplace_back(new Window(id));
					result = windows.back();
				}
				result->accessed = true;
			}
			return result;
		}
		Window* NextWindow(const char *name)		{ current.Reset(GetWindow(name)); return current.window; }
		const Widget& NextItem()					{ current.itemId++; current.index = 0; return current; }
		const Widget& NextIndex()					{ current.index++; return current; }
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

	void Init(FontMap& font){
		state.style.Font = &font;
		const GLchar *vertShaderSrc = R"(
			#version 400
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
			#version 400
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

		state.program = glCreateProgram();
		state.vertShader = glCreateShader(GL_VERTEX_SHADER);
		state.fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(state.vertShader, 1, &vertShaderSrc, 0);
		glShaderSource(state.fragShader, 1, &fragShaderSrc, 0);
		glCompileShader(state.vertShader);
		glCompileShader(state.fragShader);
		glAttachShader(state.program, state.vertShader);
		glAttachShader(state.program, state.fragShader);
		glLinkProgram(state.program);

		state.locProj = glGetUniformLocation(state.program, "proj");
		state.locTex = glGetUniformLocation(state.program, "tex");
		state.locPos = glGetAttribLocation(state.program, "pos");
		state.locUV = glGetAttribLocation(state.program, "uv");
		state.locCol = glGetAttribLocation(state.program, "col");
		
		glGenBuffers(1, &state.vbo);
		glGenBuffers(1, &state.ebo);

		glGenVertexArrays(1, &state.vao);
		glBindVertexArray(state.vao);
		glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
		glEnableVertexAttribArray(state.locPos);
		glEnableVertexAttribArray(state.locUV);
		glEnableVertexAttribArray(state.locCol);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(state.locPos, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, pos));
		glVertexAttribPointer(state.locUV, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, uv));
		glVertexAttribPointer(state.locCol, 4, GL_FLOAT, GL_TRUE, sizeof(DrawVert), (GLvoid*)OFFSETOF(DrawVert, col));
#undef OFFSETOF

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void Shutdown(){
		for (Window *w : state.windows) MemDelete(w);

		if (state.vao) glDeleteVertexArrays(1, &state.vao);
		if (state.vbo) glDeleteBuffers(1, &state.vbo);
		if (state.ebo) glDeleteBuffers(1, &state.ebo);
		state.vao = state.vbo = state.ebo = 0;

		glDeleteProgram(state.program);
		glDeleteShader(state.vertShader);
		glDeleteShader(state.fragShader);
		state.program = state.vertShader = state.fragShader = 0;
	}

	void BeginFrame(const Input& input){
		state.input = input;
		state.current.Reset();
		state.hot = state.hotToBe;
		state.hotToBe.Reset();

		for (Window *w : state.windows) w->Reset();
	}
	void EndFrame(){
		// focus on the window where the active widget is located
		if (state.active.HasValue()){
			Window *window = state.active.window;
			// move the window to the end
			if (window != state.windows.back()){
				for (int i = 0; i < state.windows.size(); i++)
					if (state.windows[i] == window)
						state.windows.erase(state.windows.begin() + i);
				state.windows.push_back(window);
			}
		}
		// delete windows we didn't touch in this frame
		state.windows.erase(
			std::remove_if(
				state.windows.begin(), state.windows.end(), 
				[](Window *w) {bool die = !w->accessed; if (die) delete w; return die; }), 
			state.windows.end());

		// ============================================================
		// backup program & texture
		GLint lastProgram, lastTexture;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		
		// setup
		const float w = state.input.screen.x;
		const float h = state.input.screen.y;
		const Matrix4x4 orthoProjection(
			2.f/w,	0.f,	0.f,	0.f,
			0.f,	2.f/-h,	0.f,	0.f,
			0.f,	0.f,	-1.f,	0.f,
			-1.f,	1.f,	0.f,	1.f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glActiveTexture(GL_TEXTURE0);

		// Render windows
		glUseProgram(state.program);
		glUniform1i(state.locTex, 0);
		glUniformMatrix4fv(state.locProj, 1, GL_FALSE, &orthoProjection[0][0]);
		glBindVertexArray(state.vao);
		glBindTexture(GL_TEXTURE_2D, state.style.Font->TexID());
		for (Window *w : state.windows){
			const DrawIdx *idxBufOffset = 0;
			DrawList& drawList = w->drawList;
			if (drawList.cmdBuf.size() > 0){
				glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
				glBufferData(GL_ARRAY_BUFFER,
					(GLsizeiptr)drawList.vtxBuf.size() * sizeof(DrawVert),
					(GLvoid*)drawList.vtxBuf.data(),
					GL_STREAM_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
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

		// restore program texture
		glDisable(GL_SCISSOR_TEST);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(lastProgram);
		glBindTexture(GL_TEXTURE_2D, lastTexture);
	}
	//	+---------------------------------------+
	//  |				1                       |
	//  +-----------------------------------+---+
	//  |                                   |   |
	//  |               2                   | 3 |
	//  |                                   |   |
	//  +-----------------------------------+---+
	//  1 - Header, 2 - Body, 3 - Scroll
	void BeginWindow(const char *name, Rect& rect){
		Window *W = state.NextWindow(name);
		float padding = state.style.WindowPadding;
		float textPadding = (padding - state.style.Font->Height()) * 0.7f;
		Rect header(rect.min, Vector2(rect.max.x, rect.min.y + padding));
		Rect body(
			Vector2(rect.min.x, rect.min.y + padding),
			Vector2(rect.max.x - padding, rect.max.y));
		Rect scroll(
			Vector2(rect.max.x - padding, rect.min.y + padding),
			Vector2(rect.max.x, rect.max.y - padding));
		if (IsActive()){
			if (CheckMouse(MouseButton::LEFT, MouseButtonState::UP)){
				ClearActive();
			}
			else {
				Rect dragArea(Vector2::ZERO, state.input.screen - Vector2(padding));
				rect.MoveTo(dragArea.ClosestPoint(state.input.mouse - state.drag));
			}
		}
		if (IsHot() && CheckMouse(MouseButton::LEFT, MouseButtonState::DOWN)){
			state.active = state.current;
			state.drag = state.input.mouse - rect.min;
		}
		if (body.Contains(state.input.mouse) || header.Contains(state.input.mouse)) {
			SetHot();
		}

		W->drawList.PushClipRect(rect);

		// Header
		W->drawList.AddTriangle(
			Vector2(rect.min.x, rect.min.y + padding),
			rect.min + Vector2(padding),
			Vector2(rect.min.x + padding, rect.min.y),
			state.style.Colors[Style::Palette::AccentHighlight]);
		W->drawList.AddRect(
			Vector2(rect.min.x + padding, rect.min.y),
			Vector2(rect.max.x, rect.min.y + padding),
			state.style.Colors[Style::Palette::AccentHighlight]);
		W->drawList.AddText(
			rect.min.x + padding + textPadding,
			rect.min.y + padding - textPadding,
			state.style.Font,
			name,
			state.style.Colors[Style::Palette::Text]);
		// Body
		W->drawList.AddRect(
			Vector2(rect.min.x, rect.min.y + padding),
			rect.max,
			state.style.Colors[Style::Palette::Background]);
	}

	void EndWindow(){
	}
	////////////////////////////////////////////////////////////////////
	// Helper implementations
	////////////////////////////////////////////////////////////////////

	bool IsHot(){ return state.hot.HasValue() && state.hot == state.current; }
	bool IsActive(){ return state.active.HasValue() && state.active == state.current; }
	void SetHot(){
		if (!state.active.HasValue() && 
			state.hotToBe != state.current && 
			state.CompareDist(state.hotToBe, state.current) > 0) {
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