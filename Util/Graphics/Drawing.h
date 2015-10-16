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
		Vec2 pos;
		Vec2 uv;
		Color	col;
	};
	struct DrawList{
		std::vector<DrawCmd>	cmdBuf;
		std::vector<DrawIdx>	idxBuf;
		std::vector<DrawVert>	vtxBuf;

		DrawIdx					vtxCurrIdx;
		DrawIdx*				idxPtr;
		DrawVert*				vtxPtr;
		std::vector<Vec2>	path;
		std::vector<Rect>		clipRectStack;

		DrawList();
		void Clear();
		void AddDrawCmd();
		void UpdateClipRect();
		void PushClipRect(const Rect& rect);
		void PushClipRectFullScreen();
		void PopClipRect();

		void AddRect(const Vec2& tl, const Vec2& br, const Color& color, bool fill = true, float thick = 1.f);
		void AddTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Color& color, bool fill = true, float thick = 1.f);
		void AddCircle(const Vec2& center, float radius, const Color& color, bool fill = true, float thick = 1.f);
		float AddText(float x, float y, const FontMap *font, const char *text, const Color& color, GlyphPosMap * posMap = nullptr);
		void AddPolyLine(const Vec2* points, const int count, const Color& color, bool closed, float thick = 1.f);
		void AddPolyFilled(const Vec2* points, const int count, const Color& color);

		inline void PathClear()													{ path.clear(); }
		inline void PathReserve(int vtxCount)									{ path.reserve(path.size() + vtxCount); }
		inline void PathFill(const Color& c)									{ AddPolyFilled(path.data(), path.size(), c); PathClear(); }
		inline void PathStroke(const Color& c, bool closed, float thick = 1.f)	{ AddPolyLine(path.data(), path.size(), c, true, thick); PathClear(); }
		inline void PathPoint(const Vec2& p)									{ 
			path.push_back(p);
		}
		inline void PathRect(const Vec2& tl, const Vec2& br)				{ 
			PathReserve(4);
			PathPoint(tl); 
			PathPoint(Vec2(br.x, tl.y)); 
			PathPoint(br); 
			PathPoint(Vec2(tl.x, br.y)); 
		}
		void PathArc(const Vec2& center, float radius, int clockPos1, int clockPos2);
	
		void PrimReserve(int idxCount, int vtxCount);
		void PrimTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Color& c);
		void PrimRect(const Vec2& tl, const Vec2& br, const Color& c);
		void PrimRectUV(const Vec2&tl, const Vec2& br, const Vec2& uvTL, const Vec2& uvBR, const Color& c);
		void PrimVtx(const Vec2& pos, const Vec2& uv, const Color& c);
		void PrimWriteVtx(const Vec2& pos, const Vec2& uv, const Color& c);
		void PrimWriteIdx(DrawIdx idx);

	};
}