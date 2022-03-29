#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <map>

#include "systems/log.h"

namespace Engine {
	namespace Loader
	{
		struct TempMesh
		{
			std::vector<Renderer3DVertex> vertices;
			std::vector<uint32_t> indices;
			std::shared_ptr<Texture> diffuseTexture = nullptr;
			glm::vec3 diffuseTint = { 1.f, 1.f, 1.f };
		};

		static std::shared_ptr<Material> s_material = nullptr;
		static Geometry s_geometry;

		static void ASSIMPProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			TempMesh tmpMesh;
			tmpMesh.vertices.reserve(mesh->mNumVertices);
			// Find vertex properties
			bool hasPositions = mesh->HasPositions();
			bool hasNormals = mesh->HasNormals();
			uint32_t numUVChannels = mesh->GetNumUVChannels();

			//Log::info("VERTICES");
			// Iterate through vertices
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				// Get vertex data
				glm::vec3 position, normal, tangent, biTangent;
				std::vector<glm::vec2> texCoords(numUVChannels);

				if (hasPositions) position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				if (hasNormals) normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

				for (uint32_t j = 0; j < numUVChannels; j++)
				{
					texCoords[j] = glm::vec2(mesh->mTextureCoords[j][i].x, mesh->mTextureCoords[j][i].y);
				}

				tmpMesh.vertices.push_back(Renderer3DVertex(position, normal, texCoords[0]));
			}

			//Log::info("INDICES");
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (uint32_t j = 0; j < face.mNumIndices; j++)
				{
					tmpMesh.indices.push_back(face.mIndices[j]);
				}

				// Log part - assume all faces are triangles and therefore have 3 indices
				//Log::info("Face {0}: {1} {2} {3}", i, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
			}
			//Log::info("Num vertices {0} Num indices {1}", mesh->mNumVertices, elementCount);

			std::vector<aiTextureType> types = {
				aiTextureType_NONE,
				aiTextureType_DIFFUSE,
				aiTextureType_SPECULAR,
				aiTextureType_AMBIENT,
				aiTextureType_EMISSIVE,
				aiTextureType_HEIGHT,
				aiTextureType_NORMALS,
				aiTextureType_SHININESS,
				aiTextureType_OPACITY,
				aiTextureType_DISPLACEMENT,
				aiTextureType_LIGHTMAP,
				aiTextureType_REFLECTION,
				aiTextureType_BASE_COLOR,
				aiTextureType_NORMAL_CAMERA,
				aiTextureType_EMISSION_COLOR,
				aiTextureType_METALNESS,
				aiTextureType_DIFFUSE_ROUGHNESS,
				aiTextureType_AMBIENT_OCCLUSION,
				aiTextureType_UNKNOWN
			};

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (auto type : types)
			{
				for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
				{
					aiString str;
					material->GetTexture(type, i, &str);
					if (type == aiTextureType_DIFFUSE)
					{
						std::string fn(str.C_Str());
						tmpMesh.diffuseTexture.reset(Texture::create(("./assets/models/lettercube/" + fn).c_str()));
					}
					//Log::info("Texture type:{0} filepath:{1}", type, str.C_Str());
				}
			}

			aiString stringValue;
			int intValue;
			float floatValue;
			aiColor3D colorValue;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, colorValue)) tmpMesh.diffuseTint = { colorValue.r, colorValue.g, colorValue.b };

			Renderer3D::addGeometry(tmpMesh.vertices, tmpMesh.indices, s_geometry);
			std::shared_ptr<Shader> shader;
			shader.reset(Shader::create("./assets/shaders/texturedPhongBatch.glsl"));
			if (tmpMesh.diffuseTexture) s_material.reset(new Material(shader, tmpMesh.diffuseTexture));
			else s_material.reset(new Material(shader, glm::vec4(tmpMesh.diffuseTint, 1.0f)));
		}

		static void ASSIMPProcessNode(aiNode* node, const aiScene* scene)
		{
			std::string parentName = "Null";
			if (node->mParent != nullptr) parentName = node->mParent->mName.C_Str();

			aiMatrix4x4* transform = &node->mTransformation;
			/*
			Log::info("TRANSFORM");
			Log::info("{0} {1} {2} {3}", transform->a1, transform->a2, transform->a3, transform->a4);
			Log::info("{0} {1} {2} {3}", transform->b1, transform->b2, transform->b3, transform->b4);
			Log::info("{0} {1} {2} {3}", transform->c1, transform->c2, transform->c3, transform->c4);
			Log::info("{0} {1} {2} {3}", transform->d1, transform->d2, transform->d3, transform->d4);
			*/
			// process all the node's meshes
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				ASSIMPProcessMesh(mesh, scene);
			}

			//  Process child nodes
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				ASSIMPProcessNode(node->mChildren[i], scene);
			}
		}

		static void ASSIMPLoad(const std::string& filepath, uint32_t flags, std::shared_ptr<Material> material, Geometry& geo) //needs geometry
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filepath, flags);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log::error("Cannot load: {0}, ASSIMP Error {1}", filepath, importer.GetErrorString());
				return;
			}
			ASSIMPProcessNode(scene->mRootNode, scene);
		}
	}
}