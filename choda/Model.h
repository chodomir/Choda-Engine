#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

#include <vector>
#include <string>

namespace choda {

	class Model
	{
	public:
		Model(const char* path);
		~Model();

		void draw(ShaderProgram &shader);
	private:
		void loadModel(const std::string &path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

	private:
		std::vector<Mesh*> meshes;
		std::string directory;
		std::vector<Texture> loadedTextures;
	};
}

