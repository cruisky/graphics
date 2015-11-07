#pragma once

#include "Util/Graphics/PrimitiveManager.h"
#include "Common.h"

namespace TX {

	class BVH : public PrimitiveManager {
	public:
		enum class SplitMethod { SAH, MIDDLE_CUT, EQUAL_COUNT};
	private:
		/// <summary>
		/// Info of a single shape (triangle).
		/// </summary>
		struct BuildData {
			uint id;			// id of the BuildTri
			Vec3 centroid;
			BBox bbox;
			BuildData(uint id, const BBox& bbox) : id(id), bbox(bbox) {
				centroid = bbox.Centroid();
			}
		};
		/// <summary>
		/// BVH tree node.
		/// </summary>
		struct BuildNode {
			Tri4 *prims;			// array of Tri4's
			BuildNode *children[2];
			BBox bounds;			// the bound box of this node
			uint16 axis;			// split axis (0/1/2)
			uint16 primCount;		// number of Tri4's used, 0: interior, 1+: leaf

			BuildNode() { children[0] = children[1] = nullptr; }
			void InitLeaf(Tri4 *triArray, uint count, const BBox& bbox) {
				children[0] = children[1] = nullptr;
				bounds = bbox;
				prims = triArray;
				primCount = count;
			}
			void InitInterior(uint16 splitAxis, BuildNode *c0, BuildNode *c1) {
				children[0] = c0;
				children[1] = c1;
				axis = splitAxis;
				bounds = Math::Union(c0->bounds, c1->bounds);
				primCount = 0;
			}
		};

		/// <summary>
		/// Flattened BVH tree node stored in depth-first order,
		/// where the first child of an elem is the elem immediately next to it.
		/// </summary>
		struct LinearNode {
			BBox bounds;
			union {
				// interior
				struct {
					uint secondChildId;
				};
				// leaf
				struct {
					uint tri4Id;
				};
			};
			uint16 axis;		// split axis (0/1/2)
			uint16 primCount;	// 0: interior, 1+: leaf
		};

	private:
		const uint					MaxTrisPerNode;
		const SplitMethod			Method;
		// Vector of vertices
		std::vector<BuildVertex>	buildVerts;
		// Vector of triangles
		std::vector<BuildTri>		buildTris;

		// Flattened BVH tree
		LinearNode*					root;
		uint						treeSize;
		// Packed array of Tri4's
		Tri4*						prims;
		uint						primCount;

	public:
		BVH(SplitMethod split = SplitMethod::MIDDLE_CUT,
			uint maxPrimsPerNode = 128) :
			Method(split),
			MaxTrisPerNode(maxPrimsPerNode) {}
		~BVH();

		bool Intersect(const Ray& ray, Intersection& isect) const;
		bool Occlude(const Ray& ray) const;
	protected:
		void Build();
	private:
		/// <summary>
		/// Fetch mesh info from primitive into buildVerts and buildTris.
		/// Assumes the underlying shape of all primitives are meshes.
		/// </summary>
		void RefineGeometry();

		/// <summary>
		/// Build BVH tree.
		/// </summary>
		/// <returns> The root </returns>
		BuildNode* RecursiveBuild(std::vector<BuildData>& buildData, uint start, uint end, MemoryArena& buildMem);

		/// <summary>
		/// Convert the BVH tree into a linear array in depth-first order.
		/// </summary>
		/// <param name="buildNode"> The BVH tree node to be converted </param>
		/// <param name="currOffset"> Pointer to the offset of current linear node </param>
		/// <param name="currPrimOffet"> Pointer to the offset of current Tri4 </param>
		/// <returns> The offset to the converted node </returns>
		uint FlattenTree(const BuildNode *buildNode, uint* currOffset, uint* currPrimOffet);
	};
}
