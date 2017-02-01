#include "stdafx.h"
#include "BVH.h"
#include "Core/Primitive.h"
#include "TXBase/Shape/Mesh.h"
#include "TXBase/Math/BBox.h"
#include <algorithm>

namespace TX {
	static inline bool IntersectBounds(const BBox& bounds, const Ray& ray, const Vec3& invDir, const Vec3u& dirSign) {
		// check for intersection against x, y slabs
		float t_min = (bounds[1 - dirSign.x].x - ray.origin.x) * invDir.x;
		float t_max = (bounds[    dirSign.x].x - ray.origin.x) * invDir.x;
		float tymin = (bounds[1 - dirSign.y].y - ray.origin.y) * invDir.y;
		float tymax = (bounds[    dirSign.y].y - ray.origin.y) * invDir.y;
		if (t_min > tymax || tymin > t_max)
			return false;
		if (tymin > t_min) t_min = tymin;
		if (tymax < t_max) t_max = tymax;

		// check for z slab
		float tzmin = (bounds[1 - dirSign.z].z - ray.origin.z) * invDir.z;
		float tzmax = (bounds[    dirSign.z].z - ray.origin.z) * invDir.z;
		if (t_min > tzmax || tzmin > t_max)
			return false;
		if (tzmin > t_min) t_min = tzmin;
		if (tzmax < t_max) t_max = tzmax;
		return t_min < ray.t_max && t_max > ray.t_min;
	}

	BVH::~BVH() {
		FreeAligned(root);
		FreeAligned(prims);
	}

	bool BVH::Intersect(const Ray& ray, Intersection& intxn) const {
		if (!root) return false;

		Vec3 invDir(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
		Vec3u dirSign(invDir.x >= 0, invDir.y >= 0, invDir.z >= 0);

		bool hit = false;
		uint todoStack[64];
		uint todoStackTop = 0, nodeId = 0;	// start from root

		while (true) {
			const LinearNode *currNode = &root[nodeId];

			// test ray intersection against the bound box of current node
			if (IntersectBounds(currNode->bounds, ray, invDir, dirSign)) {
				// leaf
				if (currNode->primCount) {
					// test ray intersection against each primitive
					for (uint i = 0; i < currNode->primCount; i++) {
						if (prims[currNode->tri4Id + i].Intersect(ray, intxn, *prims_))
							hit = true;
					}
					if (todoStackTop == 0) break;
					nodeId = todoStack[--todoStackTop];
				}
				// interior
				else {
					// let first child be the next node and push the second child
					if (dirSign[currNode->axis]) {
						todoStack[todoStackTop++] = currNode->secondChildId;
						nodeId++;
					}
					else {
						todoStack[todoStackTop++] = nodeId + 1;
						nodeId = currNode->secondChildId;
					}
				}
			}
			// missed
			else {
				// pop one node from todo stack
				if (todoStackTop == 0) break;
				nodeId = todoStack[--todoStackTop];
			}
		}
		return hit;
	}

	bool BVH::Occlude(const Ray& ray) const {
		if (!root) return false;

		Vec3 invDir(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
		Vec3u dirSign(invDir.x >= 0, invDir.y >= 0, invDir.z >= 0);

		uint todoStack[64];
		uint todoStackTop = 0, nodeId = 0;	// start from root

		while (true) {
			const LinearNode *currNode = &root[nodeId];

			// test ray intersection against the bound box of current node
			if (IntersectBounds(currNode->bounds, ray, invDir, dirSign)) {
				// leaf
				if (currNode->primCount) {
					// test ray intersection against each primitive
					for (uint i = 0; i < currNode->primCount; i++) {
						if (prims[currNode->tri4Id + i].Occlude(ray))
							return true;
					}
					if (todoStackTop == 0) break;
					nodeId = todoStack[--todoStackTop];
				}
				// interior
				else {
					// let first child be the next node and push the second child
					if (dirSign[currNode->axis]) {
						todoStack[todoStackTop++] = currNode->secondChildId;
						nodeId++;
					}
					else {
						todoStack[todoStackTop++] = nodeId + 1;
						nodeId = currNode->secondChildId;
					}
				}
			}
			// missed
			else {
				// pop one node from todo stack
				if (todoStackTop == 0) break;
				nodeId = todoStack[--todoStackTop];
			}
		}
		return false;
	}

	void BVH::Build() {
		RefineGeometry();

		if (buildTris.size() == 0) {
			root = nullptr;
			return;
		}

		// Initialize build data - array of bounding boxes
		std::vector<BuildData> buildData;
		buildData.reserve(buildTris.size());
		for (uint id = 0; id < buildTris.size(); id++) {
			const BuildTri& tri = buildTris[id];
			BBox bbox(
				buildVerts[tri.idx0].pos,
				buildVerts[tri.idx1].pos);
			bbox = Math::Union(bbox, buildVerts[tri.idx2].pos);
			buildData.emplace_back(id, bbox);
		}

		// Recursively build BVH tree
		MemoryArena buildMem;
		uint nodeCount = 0;
		std::vector<uint> orderedTris;
		orderedTris.reserve(buildTris.size());
		treeSize = 0;
		BuildNode *buildRoot = RecursiveBuild(buildData, 0, buildTris.size(), buildMem);

		// Flatten the tree into an array
		root = AllocAligned<LinearNode>(treeSize, 64);
		prims = AllocAligned<Tri4>(primCount, 16);
		//memset(root, 0, treeSize * sizeof(LinearNode));
		uint nodeOffset = 0, triOffset = 0;
		FlattenTree(buildRoot, &nodeOffset, &triOffset);
		assert(nodeOffset == treeSize);
		assert(triOffset == primCount);
	}

	void BVH::RefineGeometry() {
		// Allocate memory
		buildVerts.clear();
		buildTris.clear();
		uint vertCount = 0;
		uint triCount = 0;
		for (auto& prim : *prims_) {
			const Mesh *mesh = static_cast<const Mesh *>(prim->GetMesh());
			vertCount += mesh->VertexCount();
			triCount += mesh->TriangleCount();
		}
		buildVerts.reserve(vertCount);
		buildTris.reserve(triCount);

		// Fetch triangles and vertices from primitives
		vertCount = triCount = 0;
		for (uint primId = 0; primId < prims_->size(); primId++) {
			const Mesh *mesh = (*prims_)[primId]->GetMesh();

			// Triangles
			const uint *indices = mesh->indices.data();
			for (uint triId = 0, i = 0; triId < mesh->TriangleCount(); i = 3*(++triId)) {
				buildTris.emplace_back(
					vertCount + indices[i],
					vertCount + indices[i + 1],
					vertCount + indices[i + 2],
					primId,
					triId);
				triCount++;
			}
			// Vertices
			for (auto& vert : mesh->vertices) {
				buildVerts.push_back(vert);
			}
			vertCount += mesh->VertexCount();
		}

		assert(buildVerts.size() == vertCount);
		assert(buildTris.size() == triCount);
	}


	BVH::BuildNode* BVH::RecursiveBuild(std::vector<BuildData>& buildData, uint start, uint end, MemoryArena& buildMem) {
		struct CompareToMid {
			int dim;
			float mid;
			CompareToMid(int dim, float mid) : dim(dim), mid(mid) {}
			bool operator()(const BuildData& a) {
				return a.centroid[dim] < mid;
			}
		};
		struct ComparePoints {
			int dim;
			ComparePoints(int dim) : dim(dim) {}
			bool operator()(const BuildData& a, const BuildData& b) {
				return a.centroid[dim] < b.centroid[dim];
			}
		};

		assert(start != end);
		treeSize++;
		const uint triCount = end - start;
		const uint tri4Count = (triCount + 3) / 4;
		BuildNode *node = buildMem.Alloc<BuildNode>();

		auto BuildLeaf = [&] {
			uint triId = start;
			Tri4 *tri4Array = buildMem.Alloc<Tri4>(tri4Count);
			BBox bounds;
			for (uint i = 0; i < tri4Count; i++) {
				const BuildVertex *vertices[4][3];
				const BuildTri *triangles[4];
				uint count = 0;
				for (; count < 4 && triId < end; count++, triId++) {
					const BuildData& triInfo = buildData[triId];
					const BuildTri& tri = buildTris[triInfo.id];
					bounds = Math::Union(bounds, triInfo.bbox);
					triangles[count] = &tri;
					vertices[count][0] = &(buildVerts[tri.idx0]);
					vertices[count][1] = &(buildVerts[tri.idx1]);
					vertices[count][2] = &(buildVerts[tri.idx2]);
				}

				tri4Array[i] = Tri4();
				tri4Array[i].Pack(vertices, triangles, count);
			}
			node->InitLeaf(tri4Array, tri4Count, bounds);
			primCount += tri4Count;
		};

		if (tri4Count == 1) {
			// Create leaf node
			BuildLeaf();
			return node;
		}
		else {
			// Create interior node
			// Compute bound of centroids, choose split dimension
			BBox centroidBounds;
			for (uint i = start; i < end; i++)
				centroidBounds = Math::Union(centroidBounds, buildData[i].centroid);
			int dim = centroidBounds.MaximumExtent();

			// Partition primitives into two sets and build children
			uint mid = (start + end) / 2;
			// All bounding boxes are concentric
			if (centroidBounds.max[dim] == centroidBounds.min[dim]) {
				if (primCount <= MaxTrisPerNode) {
					// All the triangles can be stored in one node
					BuildLeaf();
					return node;
				}
				else {
					// Need to split it furthur
					node->InitInterior(
						dim,
						RecursiveBuild(buildData, start, mid, buildMem),
						RecursiveBuild(buildData, mid, end, buildMem));
					return node;
				}
			}

			switch (Method) {
			case SplitMethod::MIDDLE_CUT:
			{
				float midpoint = 0.5f * (centroidBounds.min[dim] + centroidBounds.max[dim]);
				auto midPtr = std::partition(
					&buildData[start],
					&buildData[end - 1] + 1,
					CompareToMid(dim, midpoint));
				mid = midPtr - &buildData[0];
			}
				if (mid != start && mid != end)
					//
					break;
			case SplitMethod::EQUAL_COUNT:
				mid = (start + end) / 2;
				std::nth_element(
					&buildData[start],
					&buildData[mid],
					&buildData[end - 1] + 1,
					ComparePoints(dim));
				break;
			case SplitMethod::SAH:
				break;
			}
			node->InitInterior(
				dim,
				RecursiveBuild(buildData, start, mid, buildMem),
				RecursiveBuild(buildData, mid, end, buildMem));
			return node;
		}
	}

	uint BVH::FlattenTree(const BuildNode *buildNode, uint* currOffset, uint* currPrimOffset) {
		uint nodeOffset = (*currOffset)++;
		LinearNode *currNode = root + nodeOffset;
		currNode->bounds = buildNode->bounds;
		currNode->axis = buildNode->axis;
		currNode->primCount = buildNode->primCount;
		// Leaf case
		if (buildNode->primCount) {
			assert(!buildNode->children[0] && !buildNode->children[1]);
			// copy the Tri4's to the flattened tree
			currNode->tri4Id = *currPrimOffset;
			for (uint i = 0; i < buildNode->primCount; i++) {
				uint primOffset = (*currPrimOffset)++;
				prims[primOffset] = buildNode->prims[i];
			}
		}
		// Interior case
		else {
			// first child
			FlattenTree(buildNode->children[0], currOffset, currPrimOffset);
			// second child
			currNode->secondChildId = FlattenTree(buildNode->children[1], currOffset, currPrimOffset);
		}
		return nodeOffset;
	}
}
