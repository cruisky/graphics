#pragma

#include "Util.h"
#include "Math/Geometry.h"
#include "Color.h"
#include "FontMap.h"

namespace TX
{
	typedef uint16 DrawIdx;
	struct DrawCmd{
		DrawIdx	idxCount;
		Rect	clipRect;
	};
	struct DrawVert{
		Vector2 pos;
		Vector2 uv;
		Color	col;
	};
	struct DrawList{
		std::vector<DrawCmd>	cmdBuf;
		std::vector<DrawIdx>	idxBuf;
		std::vector<DrawVert>	vtxBuf;

		DrawIdx					vtxCurrIdx;
		DrawIdx*				idxPtr;
		DrawVert*				vtxPtr;
		std::vector<Vector2>	path;
		std::vector<Rect>		clipRectStack;

		DrawList();
		void Clear();
		void AddDrawCmd();
		void UpdateClipRect();
		void PushClipRect(const Rect& rect);
		void PushClipRectFullScreen();
		void PopClipRect();

		void AddRect(const Vector2& tl, const Vector2& br, const Color& color, bool fill = true, float thick = 1.f);
		void AddTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& color, bool fill = true, float thick = 1.f);
		Vector2 AddText(float x, float y, const FontMap *font, const char *text, const Color& color);
		void AddPolyLine(const Vector2* points, const int count, const Color& color, bool closed, float thick = 1.f);
		void AddPolyFilled(const Vector2* points, const int count, const Color& color);

		inline void PathClear()													{ path.clear(); }
		inline void PathReserve(int vtxCount)									{ path.reserve(path.size() + vtxCount); }
		inline void PathFill(const Color& c)									{ AddPolyFilled(path.data(), path.size(), c); PathClear(); }
		inline void PathStroke(const Color& c, bool closed, float thick = 1.f)	{ AddPolyLine(path.data(), path.size(), c, true, thick); PathClear(); }
		inline void PathPoint(const Vector2& p)									{ 
			path.push_back(p);
		}
		inline void PathRect(const Vector2& tl, const Vector2& br)				{ 
			PathReserve(4);
			PathPoint(tl); 
			PathPoint(Vector2(br.x, tl.y)); 
			PathPoint(br); 
			PathPoint(Vector2(tl.x, br.y)); 
		}
	
		void PrimReserve(int idxCount, int vtxCount);
		void PrimRect(const Vector2& tl, const Vector2& br, const Color& c);
		void PrimRectUV(const Vector2&tl, const Vector2& br, const Vector2& uvTL, const Vector2& uvBR, const Color& c);
		void PrimVtx(const Vector2& pos, const Vector2& uv, const Color& c);
		void PrimWriteVtx(const Vector2& pos, const Vector2& uv, const Color& c);
		void PrimWriteIdx(DrawIdx idx);

	};
}