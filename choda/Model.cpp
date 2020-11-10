#include "Model.h"

choda::Model::Model(const char* path)
{
	loadModel(path);
}

choda::Model::~Model()
{
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

void choda::Model::draw(ShaderProgram& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->draw(shader);
	}
}

void choda::Model::loadModel(const std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::string("ERROR::ASSIMP::", import.GetErrorString());
	}

	directory = path.substr(0, path.find_last_of('\\'));

	processNode(scene->mRootNode, scene);
}

void choda::Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

choda::Mesh* choda::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// TODO: this can be avoided (a lot of copies, lot of data)
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// vertices, normals and texture coordinates
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if (mesh->HasNormals()) {
			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);
		}

		// TODO: Only using first set of texture coordinates
		if (mesh->HasTextureCoords(0)) {
			texCoords.push_back(mesh->mTextureCoords[0][i].x);
			texCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			texCoords.push_back(0.0f);
			texCoords.push_back(0.0f);
		}
	}
	
	// indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		auto specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), diffuse.begin(), diffuse.end());
		textures.insert(textures.end(), specular.begin(), specular.end());
	}

	return new Mesh(vertices, normals, texCoords, indices, textures);
}

std::vector<choda::Texture> choda::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		// check if texture is already loaded
		for (int j = 0; j < loadedTextures.size(); j++) {
			if (strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
				skip = true;
				textures.push_back(loadedTextures[j]);
				break;
			}
		}

		if (!skip) {
			Texture texture;
			texture.id = Texture::GenerateFromFile((directory + "\\" + str.C_Str()).c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture); // push the texture into list of loaded textures
		}
	}
	return textures;
}

