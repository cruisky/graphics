#include "UtilStdAfx.h"
#include "Geometry.h"
#include "System/Memory.h"

namespace TX
{
	bool Polygon::IsConvex(const Vec2& v1, const Vec2& v2, const Vec2& v3){
		return ((v3.y - v1.y) * (v2.x - v1.x)) > ((v3.x - v1.x) * (v2.y - v1.y));
	}

	void Polygon::Triangulate(uint16 *idxBuf, uint16 idxOffset) const {
		int vertCount = Count();
		if (vertCount < 3) return;
		if (vertCount == 3) {
			idxBuf[0] = idxOffset;
			idxBuf[1] = idxOffset + 1;
			idxBuf[2] = idxOffset + 2;
			return;
		}

		auto ClipEar = [&idxBuf, idxOffset](Vertex *ear){
			idxBuf[0] = idxOffset + ear->prev->id;
			idxBuf[1] = idxOffset + ear->id;
			idxBuf[2] = idxOffset + ear->next->id;
			idxBuf += 3;
			ear->active = false;
			ear->prev->next = ear->next;
			ear->next->prev = ear->prev;
		};

		// initialize vertex list
		Vertex *verts = new Vertex[vertCount];
		AutoDeleteArray<Vertex> _(verts);
		for (int i = 0; i < vertCount; i++){
			verts[i].id = i;
			verts[i].p = &points[i];
			if (i == vertCount - 1) verts[i].next = verts;
			else verts[i].next = &verts[i + 1];
			if (i == 0) verts[i].prev = &verts[vertCount - 1];
			else verts[i].prev = &verts[i - 1];
		}
		for (int i = 0; i < vertCount; i++)
			UpdateVertex(verts[i], verts, vertCount);

		// clip (count - 3) ears
		for (int i = 0; i < vertCount - 3; i++){
			Vertex *ear = nullptr;
			
			// find the most extruded ear
			for (int j = 0; j < vertCount; j++){
				if (verts[j].ear){
					if (!ear){
						ear = &verts[j];
					}
					else {
						if (verts[i].cosine > ear->cosine)
							ear = &verts[i];
					}
				}
			}
			if (!ear) return;
			ClipEar(ear);
			if (i != vertCount - 4){
				UpdateVertex(*ear->prev, verts, vertCount);
				UpdateVertex(*ear->next, verts, vertCount);
			}
		}

		// add the last triangle
		for (int i = 0; i < vertCount; i++){
			if (verts[i].active){
				ClipEar(verts + i);
				break;
			}
		}
	}

	void Polygon::UpdateVertex(Vertex& v2, Vertex *verts, int count) {
		Vertex &v1 = *v2.prev, &v3 = *v2.next;
		v2.convex = IsConvex(*v1.p, *v2.p, *v3.p);
		v2.cosine = Math::Dot(
			Math::Normalize(*v1.p - *v2.p),
			Math::Normalize(*v3.p - *v2.p));
		if (v2.convex){
			v2.ear = true;
			for (int i = 0; i < count; i++){
				// skip current vert and its neighbors
				if (verts[i].p == v1.p || 
					verts[i].p == v2.p || 
					verts[i].p == v3.p) 
					continue;
				// if any other point falls into this triangle, then v2 cannot be removed
				if (IsConvex(*v1.p, *verts[i].p, *v2.p) &&
					IsConvex(*v2.p, *verts[i].p, *v3.p) &&
					IsConvex(*v3.p, *verts[i].p, *v1.p))
				{
					v2.ear = false;
					break;
				}
			}
		}
		else{
			v2.ear = false;
		}
	}
}