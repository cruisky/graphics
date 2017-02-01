#include "TXBase/stdafx.h"
#include "Drawing.h"

namespace TX
{
	static Rect NullClipRect(-1e5f, -1e5f, 1e5f, 1e5f);

	DrawList::DrawList(){
		vtxCurrIdx = 0;
		idxPtr = idxBuf.data();
		vtxPtr = vtxBuf.data();
	}

	void DrawList::Clear(){
		cmdBuf.clear();
		idxBuf.clear();
		vtxBuf.clear();
		vtxCurrIdx = 0;
		vtxPtr = nullptr;
		idxPtr = nullptr;
		clipRectStack.clear();
		PathClear();
	}
	void DrawList::AddDrawCmd(){
		DrawCmd cmd;
		cmd.idxCount = 0;
		cmd.clipRect = clipRectStack.size() ? clipRectStack.back() : NullClipRect;
		assert(cmd.clipRect.Valid());
		cmdBuf.push_back(cmd);
	}

	void DrawList::UpdateClipRect(){
		DrawCmd *cmd = cmdBuf.size() ? &cmdBuf.back() : nullptr;
		if (!cmd || cmd->idxCount != 0){	// push first cmd / pop cmd
			AddDrawCmd();
		}
		else {
			const Rect& clipRect = clipRectStack.size() ? clipRectStack.back() : NullClipRect;
			if (cmdBuf.size() > 1 && Math::Length(Vec4(cmdBuf.data()[cmdBuf.size() - 2].clipRect.f) - Vec4(clipRect.f)) < 1e-4f)
				cmdBuf.pop_back();			// avoid duplicate
			else
				cmd->clipRect = clipRect;	// do nothing
		}
	}

	void DrawList::PushClipRect(const Rect& rect){
		if (clipRectStack.empty()){
			clipRectStack.push_back(rect);
		}
		else {
			// restrict clip rect to inside the previous one
			clipRectStack.push_back(Rect(rect).Clip(clipRectStack.back()));
		}
		UpdateClipRect();
	}
	void DrawList::PushClipRectFullScreen(){
		PushClipRect(NullClipRect);
	}
	void DrawList::PopClipRect(){
		assert(clipRectStack.size() > 0);
		clipRectStack.pop_back();
		UpdateClipRect();
	}

	void DrawList::AddRect(const Vec2& tl, const Vec2& br, const Color& color, bool fill, float thick){
		if (color.a == 0.f) return;
		if (fill){
			PrimReserve(6, 4);
			PrimRect(tl, br, color);
		}
		else {
			PathRect(tl + Vec2(0.5f), br + Vec2(0.5f));
			PathStroke(color, true, thick);
		}
	}
	void DrawList::AddTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Color& color, bool fill, float thick){
		if (color.a == 0.f) return;

		if (fill){
			PrimReserve(3, 3);
			PrimTriangle(v1, v2, v3, color);
		}
		else {
			PathPoint(v1); PathPoint(v2); PathPoint(v3);
			PathStroke(color, true, thick);
		}
	}
	void DrawList::AddCircle(const Vec2& center, float radius, const Color& color, bool fill, float thick){
		if (color.a == 0.f) return;
		PathArc(center, radius, 0, 11);
		if (fill){
			PathFill(color);
		}
		else {
			PathStroke(color, true, thick);
		}
	}

	float DrawList::AddText(float x, float y, const FontMap *font, const std::string& text, const Color& color, GlyphPosMap * posMap){
		if (color.a == 0.f) return x;
		if (posMap)
			posMap->Clear();
		Vec2 pos(x, y);
		Rect rect, uv;
		int length = text.length();
		PrimReserve(length * 6, length * 4);
		for (const char& c : text) {
			if (font->GetChar(c, pos, &rect, &uv, posMap)){
				PrimRectUV(rect.min, rect.max, uv.min, uv.max, color);
			}
		}
		return pos.x - x;
	}
	void DrawList::AddPolyLine(const Vec2* points, const int count, const Color& color, bool closed, float thick){
		if (color.a == 0.f || count < 2) return;

		int lineCount = closed ? count : count - 1;
		PrimReserve(lineCount * 6, lineCount * 4);
		Vec2 uv(Vec2::ZERO);
		for (int i = 0; i < lineCount; i++){
			int j = i + 1;
			if (j == count) j = 0;
			const Vec2& p1 = points[i];
			const Vec2& p2 = points[j];
			Vec2 dir(p2 - p1);
			dir = Math::Normalize(dir);
			const Vec2 d = dir * (thick * 0.5f);

			vtxPtr[0].pos.x = p1.x + d.y; vtxPtr[0].pos.y = p1.y - d.x; vtxPtr[0].uv = uv; vtxPtr[0].col = color;
			vtxPtr[1].pos.x = p2.x + d.y; vtxPtr[1].pos.y = p2.y - d.x; vtxPtr[1].uv = uv; vtxPtr[1].col = color;
			vtxPtr[2].pos.x = p2.x - d.y; vtxPtr[2].pos.y = p2.y + d.x; vtxPtr[2].uv = uv; vtxPtr[2].col = color;
			vtxPtr[3].pos.x = p1.x - d.y; vtxPtr[3].pos.y = p1.y + d.x; vtxPtr[3].uv = uv; vtxPtr[3].col = color;
			vtxPtr += 4;

			idxPtr[0] = vtxCurrIdx; idxPtr[1] = vtxCurrIdx + 2; idxPtr[2] = vtxCurrIdx + 1;
			idxPtr[3] = vtxCurrIdx; idxPtr[4] = vtxCurrIdx + 3; idxPtr[5] = vtxCurrIdx + 2;

			idxPtr += 6;
			vtxCurrIdx += 4;
		}
	}
	void DrawList::AddPolyFilled(const Vec2* points, const int vtxCount, const Color& color){
		if (color.a == 0.f || vtxCount < 3) return;

		const Vec2& uv = Vec2::ZERO;
		const int idxCount = (vtxCount - 2) * 3;

		PrimReserve(idxCount, vtxCount);

		Polygon p(points, vtxCount);
		p.Triangulate(idxPtr, vtxCurrIdx);
		idxPtr += idxCount;

		for (int i = 0; i < vtxCount; i++, vtxPtr++){
			vtxPtr[0].pos = points[i];
			vtxPtr[0].col = color;
			vtxPtr[0].uv = uv;
		}
		vtxCurrIdx += vtxCount;
	}

	void DrawList::PathArc(const Vec2& center, float radius, int clockPos1, int clockPos2){
		static const int VTX_COUNT = 12;
		static Vec2 circle[VTX_COUNT];
		static bool circleInit = false;

		if (!circleInit){
			for (int i = 0; i < VTX_COUNT; i++){
				float deg = float(i) / VTX_COUNT * (2 * Math::PI);
				circle[i].x = Math::Cos(deg);
				circle[i].y = Math::Sin(deg);
			}
			circleInit = true;
		}
		if (clockPos1 > clockPos2) return;
		if (radius < Math::EPSILON){
			path.push_back(center);
		}
		else {
			PathReserve(clockPos2 - clockPos1 + 1);
			for (int pos = clockPos1; pos <= clockPos2; pos++){
				const Vec2& dir = circle[pos % VTX_COUNT];
				PathPoint(center + dir * radius);
			}
		}
	}


	void DrawList::PrimReserve(int idxCount, int vtxCount){
		DrawCmd& cmd = cmdBuf.back();
		cmd.idxCount += idxCount;

		int vtxBufSize = vtxBuf.size();
		vtxBuf.resize(vtxBufSize + vtxCount);
		vtxPtr = vtxBuf.data() + vtxBufSize;

		int idxBufSize = idxBuf.size();
		idxBuf.resize(idxBufSize + idxCount);
		idxPtr = idxBuf.data() + idxBufSize;
	}
	void DrawList::PrimTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Color& c){
		idxPtr[0] = vtxCurrIdx;
		idxPtr[1] = vtxCurrIdx + 1;
		idxPtr[2] = vtxCurrIdx + 2;
		idxPtr += 3;
		PrimWriteVtx(v1, Vec2::ZERO, c);
		PrimWriteVtx(v2, Vec2::ZERO, c);
		PrimWriteVtx(v3, Vec2::ZERO, c);
	}

	void DrawList::PrimRect(const Vec2& tl, const Vec2& br, const Color& c){
		idxPtr[0] = vtxCurrIdx;
		idxPtr[1] = vtxCurrIdx + 2;
		idxPtr[2] = vtxCurrIdx + 1;
		idxPtr[3] = vtxCurrIdx;
		idxPtr[4] = vtxCurrIdx + 3;
		idxPtr[5] = vtxCurrIdx + 2;
		idxPtr += 6;
		PrimWriteVtx(tl, Vec2::ZERO, c);
		PrimWriteVtx(Vec2(br.x, tl.y), Vec2::ZERO, c);
		PrimWriteVtx(br, Vec2::ZERO, c);
		PrimWriteVtx(Vec2(tl.x, br.y), Vec2::ZERO, c);
	}
	void DrawList::PrimRectUV(const Vec2&tl, const Vec2& br, const Vec2& uvTL, const Vec2& uvBR, const Color& c){
		idxPtr[0] = vtxCurrIdx;
		idxPtr[1] = vtxCurrIdx + 2;
		idxPtr[2] = vtxCurrIdx + 1;
		idxPtr[3] = vtxCurrIdx;
		idxPtr[4] = vtxCurrIdx + 3;
		idxPtr[5] = vtxCurrIdx + 2;
		idxPtr += 6;
		PrimWriteVtx(tl, uvTL, c);
		PrimWriteVtx(Vec2(br.x, tl.y), Vec2(uvBR.x, uvTL.y), c);
		PrimWriteVtx(br, uvBR, c);
		PrimWriteVtx(Vec2(tl.x, br.y), Vec2(uvTL.x, uvBR.y), c);
	}
	void DrawList::PrimVtx(const Vec2& pos, const Vec2& uv, const Color& c){
		PrimWriteIdx(vtxCurrIdx); PrimWriteVtx(pos, uv, c);
	}
	void DrawList::PrimWriteVtx(const Vec2& pos, const Vec2& uv, const Color& c){
		vtxPtr->pos = pos;
		vtxPtr->uv = uv;
		vtxPtr->col = c;
		vtxPtr++;
		vtxCurrIdx++;
	}
	void DrawList::PrimWriteIdx(DrawIdx idx){
		*idxPtr = idx; idxPtr++;
	}

}
