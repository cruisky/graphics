#include "UtilStdAfx.h"
#include "Obj.h"
#include "Libs/tiny_obj_loader.h"

namespace TX
{
	void ObjMesh::LoadQuad(float length, const Vec3& position, const Vec3& rotation, const Vec3& scale){
		//TODO
	}

	void ObjMesh::LoadSphere(float radius, const Vec3& position, const Vec3& rotation, const Vec3& scale){
		//TODO
	}

	void ObjMesh::Clear(){
		Mesh::Clear();
		materials.clear();
	}


	void ObjLoader::ConvertObj(const void * src, ObjShape& dest){
		const tinyobj::shape_t& origin = *static_cast<const tinyobj::shape_t *>(src);
		dest.mesh.Clear();
		dest.name = origin.name;

		assert((origin.mesh.positions.size() % 3) == 0);
		dest.mesh.vertices.reserve(origin.mesh.positions.size() / 3);
		for (uint i = 0; i < origin.mesh.positions.size(); i += 3){
			dest.mesh.vertices.emplace_back(
				origin.mesh.positions[i],
				origin.mesh.positions[i + 1],
				origin.mesh.positions[i + 2]);
		}

		assert((origin.mesh.normals.size() % 3) == 0);
		dest.mesh.normals.reserve(origin.mesh.normals.size() / 3);
		for (uint i = 0; i < origin.mesh.normals.size(); i += 3){
			dest.mesh.normals.emplace_back(
				origin.mesh.normals[i],
				origin.mesh.normals[i + 1],
				origin.mesh.normals[i + 2]);
		}

		assert((origin.mesh.texcoords.size() % 2) == 0);
		dest.mesh.uv.reserve(origin.mesh.texcoords.size() / 2);
		for (uint i = 0; i < origin.mesh.texcoords.size(); i += 2){
			dest.mesh.uv.emplace_back(
				origin.mesh.texcoords[i],
				origin.mesh.texcoords[i + 1]);
		}

		assert((origin.mesh.indices.size() % 3) == 0);
		dest.mesh.indices.reserve(origin.mesh.indices.size());
		dest.mesh.indices.assign(origin.mesh.indices.begin(), origin.mesh.indices.end());

		assert(origin.mesh.material_ids.size() == origin.mesh.indices.size() / 3);
		dest.mesh.materials.reserve(origin.mesh.material_ids.size());
		dest.mesh.materials.assign(origin.mesh.material_ids.begin(), origin.mesh.material_ids.end());
	}

	void ObjLoader::ConvertMtl(const void * src, ObjMaterial& dest){
		const tinyobj::material_t& origin = *static_cast<const tinyobj::material_t *>(src);
		dest.ambient = Color(origin.ambient[0], origin.ambient[1], origin.ambient[2]);
		dest.diffuse = Color(origin.diffuse[0], origin.diffuse[1], origin.diffuse[2]);
		dest.specular = Color(origin.specular[0], origin.specular[1], origin.specular[2]);
		dest.transmittance = Color(origin.transmittance[0], origin.transmittance[1], origin.transmittance[2]);
		dest.emission = Color(origin.emission[0], origin.emission[1], origin.emission[2]);
		dest.shininess = origin.shininess;
		dest.ior = origin.ior;
		dest.dissolve = origin.dissolve;

		dest.name = origin.name;
		dest.texAmbient = origin.ambient_texname;
		dest.texDiffuse = origin.diffuse_texname;
		dest.texSpecular = origin.specular_texname;
		dest.texSpecularHighlight = origin.specular_highlight_texname;
		dest.texBump = origin.bump_texname;
		dest.texDisplacement = origin.displacement_texname;
		dest.texAlpha = origin.alpha_texname;
	}

	void ObjLoader::Load(std::vector<ObjShape>& objects, std::vector<ObjMaterial>& materials, const char *objFile, const char *mtlDir){
		std::vector<tinyobj::shape_t> objs;
		std::vector<tinyobj::material_t> mtls;
		std::string error = tinyobj::LoadObj(objs, mtls, objFile, mtlDir);
		if (!error.empty()){
			throw error;
		}

		objects.resize(objs.size());
		for (uint i = 0; i < objs.size(); i++)
			ConvertObj(&objs[i], objects[i]);

		materials.resize(mtls.size());
		for (uint i = 0; i < mtls.size(); i++)
			ConvertMtl(&mtls[i], materials[i]);
	}
}
